#! /bin/bash
# vim:ts=4:sts=4:sw=4:tw=80:expandtab

function cleanup() {
    rm "${TMP:?}";
}

trap cleanup 1 2 3 9 15;

function get_class_file() {
    sed 's/\r/\n/g' "${ATP_IN:?}" |
    xargs echo |
    sed -n -e 's@.*<classPath>[[:blank:]]*<file>\([^>]\+\)</file>[[:blank:]]*</classPath>.*@\1@p';
}

readonly DEFAULT_COLOUR_METHOD=RGB_index;
readonly DEFAULT_NORMAL_WIDTH=4;
readonly DEFAULT_FILL_ALPHA=0.5;

function usage() {
    readonly local EXIT_CODE="${1:-0}";
    echo -n -e "${2:+${2:?}\n\n}" > /dev/stderr;

    cat > /dev/stderr <<-EOF
	Usage: $0 arguments

	REQUIRED ARGUMENT:
	 -p <project.atp>

	HELP:
	 -h

	OPTIONAL ARGUMENTS:         VALUES    DEFAULT
	 -c <colour method>         RGB_index ${DEFAULT_COLOUR_METHOD:?}
	                            HSL_space
	                            HSV_space
	                            RGB_space
	 -w <normal border width>   ≥ 1       ${DEFAULT_NORMAL_WIDTH:?}
	 -W <selected border width> ≥ 1       2·<normal border width>
	 -a <fill alpha>:           [0, 1]    ${DEFAULT_FILL_ALPHA}

	OUTPUT FILE:
	 <project>.visualize_classes.<colour_method>.atp
	EOF

    exit ${EXIT_CODE:?};
}

while getopts ":hp:c:w:W:a:" OPTION; do
    case "${OPTION:?}" in
        h) usage;;
        p) ATP_IN="${OPTARG:?}";;
        c) COLOUR_METHOD="${OPTARG:?}";;
        w) NORMAL_WIDTH="${OPTARG:?}";;
        W) SELECTED_WIDTH="${OPTARG:?}";;
        a) FILL_ALPHA="${OPTARG:?}";;
        ?) usage 1 "Fatal: unrecognized argument ${OPTARG:?}";;
    esac;
done;

[ "${ATP_IN}" ] ||
usage 2 "Fatal: no project file given";

readonly DIR=$(dirname "${0:?}");
readonly ATC_IN=$(dirname "${ATP_IN:?}")/$(get_class_file);
readonly RGB_index="${DIR:?}/RGB_index.txt";
readonly TMP=$(mktemp);

ATP_OUT="${ATP_IN%.*}.visualize_classes.${COLOUR_METHOD:-${DEFAULT_COLOUR_METHOD:?}}.atp";
echo -e "\n${ATP_OUT:?}" > /dev/stderr;
gawk --re-interval                         \
     --non-decimal-data                    \
     -f "${DIR:?}/library.awk"             \
     -f "${DIR:?}/xml_library.awk"         \
     -f "${DIR:?}/visualize_classes.awk"  \
     -v "colour_method=${COLOUR_METHOD:-${DEFAULT_COLOUR_METHOD:?}}"   \
     -v "RGB_index_filename=${RGB_index}"           \
     -v "normal_width=${NORMAL_WIDTH:-${DEFAULT_NORMAL_WIDTH:?}}"  \
     -v "selected_width=${SELECTED_WIDTH:-$(expr ${NORMAL_WIDTH:-${DEFAULT_NORMAL_WIDTH:?}} \* 2)}" \
     -v "fill_alpha=${FILL_ALPHA:-${DEFAULT_FILL_ALPHA:?}}"    \
     "${ATC_IN:?}" > "${TMP:?}" &&
sed -e "/<annoProject\>/r ${TMP:?}" "${ATP_IN:?}" > "${ATP_OUT}";
