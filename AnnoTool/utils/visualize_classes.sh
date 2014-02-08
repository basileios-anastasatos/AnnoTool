#! /bin/bash
# vim:ts=4:sts=4:sw=4:tw=80:expandtab

function cleanup() {
    rm "${TMP:?}";
}

trap cleanup 1 2 3 9 15;

readonly DIR=$(dirname "${0:?}");

function get_class_file() {
    sed 's/\r/\n/g' "${ATP_IN:?}" |
    xargs echo |
    sed -n -e 's@.*<classPath>[[:blank:]]*<file>\([^>]\+\)</file>[[:blank:]]*</classPath>.*@\1@p';
}

function usage() {
    [ "${@}" ] &&
    echo "Fatal: ${@:?}" > /dev/stderr;

    cat > /dev/stderr <<-EOF
	usage: $0 options

	OPTIONS:
	 -h                         show this message
	 -p <project.atp>           (required)
	 -c <colour spaces>         (default: HSL HSV RGB)
	 -w <normal border width>   (default: 4)
	 -W <selected border width> (default: twice the normal width)
	 -a <fill alpha>:           (default: 0.5)
	EOF

    if [ "${@}" ]; then
        exit 1;
    else
        exit 0;
    fi;
}

while getopts ":hp:c:w:W:a:" OPTION; do
    case "${OPTION:?}" in
        h) usage;;
        p) ATP_IN="${OPTARG:?}";;
        c) COLOUR_SPACES="${OPTARG:?}";;
        w) NORMAL_WIDTH="${OPTARG:?}";;
        W) SELECTED_WIDTH="${OPTARG:?}";;
        a) FILL_ALPHA="${OPTARG:?}";;
        ?) usage "unrecognized argument ${OPTARG:?}";;
    esac;
done;

[ "${ATP_IN}" ] ||
usage "no project file given";

readonly ATC_IN=$(dirname "${ATP_IN:?}")/$(get_class_file);
readonly TMP=$(mktemp);

for COLOUR_SPACE in ${COLOUR_SPACES:-HSL HSV RGB}; do
    ATP_OUT="${ATP_IN%.*}.visualize_classes.${COLOUR_SPACE:?}.atp";
    echo -e "\n${ATP_OUT:?}" > /dev/stderr;
    "${DIR:?}"/visualize_classes.awk         \
       -v "colour_space=${COLOUR_SPACE:?}"   \
       -v "normal_width=${NORMAL_WIDTH:-4}"  \
       -v "selected_width=${SELECTED_WIDTH}" \
       -v "fill_alpha=${FILL_ALPHA:-0.5}"    \
       "${ATC_IN:?}" > "${TMP:?}" &&
    sed '/<annoProject\>/r '"${TMP:?}" "${ATP_IN:?}" > "${ATP_OUT}";
done;
