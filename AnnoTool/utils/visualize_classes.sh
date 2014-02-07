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

readonly DIR=$(dirname "${0:?}");
readonly ATP_IN="${1:?}"; shift;
readonly ATC_IN=$(dirname "${ATP_IN:?}")/$(get_class_file);
readonly TMP=$(mktemp);
readonly COLOUR_SPACES="${1:-HSL HSV RGB}"; shift;

for COLOUR_SPACE in ${COLOUR_SPACES:?}; do
    ATP_OUT="${ATP_IN%.*}.visualize_classes.${COLOUR_SPACE:?}.atp";
    echo -e "\n${ATP_OUT:?}" > /dev/stderr;
    "${DIR:?}"/visualize_classes.awk \
       -v "colour_space=${COLOUR_SPACE:?}" "${ATC_IN:?}" > "${TMP:?}";
    sed '/<annoProject\>/r '"${TMP:?}"      "${ATP_IN:?}" > "${ATP_OUT}";
done;
