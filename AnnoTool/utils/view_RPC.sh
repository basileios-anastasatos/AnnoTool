#! /bin/bash
# vim:ts=4:sts=4:sw=4:tw=80:expandtab

function process() {
    echo "DATA ${1:?}";
    if [ -f "${2:?}.idl.gz" ]; then
        zcat "${2:?}.idl.gz";
    else
        cat "${2:?}";
    fi |
    sed -e 's@, \([-0-9]\)@,\1@g' \
        -e 's@ @\n@g';
}

readonly DIR=$(dirname "${0:?}");

readonly PREFIX="/tmp/RPC_${USER:?}";

function cleanup() {
    rm "${PREFIX:?}-truePositives.idl.gz"                \
       "${PREFIX:?}-falsePositives.idl.gz"               \
       "${PREFIX:?}-missingRecall.idl.gz"                \
       "${PREFIX:?}-truePositives-sortedByScore.idl.gz"  \
       "${PREFIX:?}-falsePositives-sortedByScore.idl.gz" \
       "$(dirname "${DETECTION:?}")/rpc-$(basename "${DETECTION:?}" .idl).txt";
}

trap cleanup 1 2 3 9 15;

readonly GROUND_TRUTH="${1:?}"; shift;
readonly DETECTION="${1:?}";    shift;
readonly ATP="${1:?}";          shift;

readonly ATP_DIR=$(dirname "${ATP:?}");
readonly ATC="${ATP_DIR:?}/classes.atc";
readonly ANNOTATIONS_DIR="${ATP_DIR:?}/annotations";

doRPC.py "${GROUND_TRUTH:?}" \
         "${DETECTION:?}" \
         -a "${PREFIX:?}" &&
mkdir -p "${ANNOTATIONS_DIR:?}" &&
{
    process G "${GROUND_TRUTH:?}";
    process D "${DETECTION:?}";
    process T "${PREFIX:?}-truePositives";
    process F "${PREFIX:?}-falsePositives";
    process M "${PREFIX:?}-missingRecall";
} |
gawk -f "${DIR:?}/library.awk"                 \
     -f "${DIR:?}/xml_library.awk"             \
     -f "${DIR:?}/show_RPC.awk"                \
     -v "atp=${ATP:?}"                         \
     -v "atc=${ATP_DIR:?}/classes.atc"         \
     -v "annotations_dir=${ANNOTATIONS_DIR:?}" \
     -v "normal_width=4"                       \
     -v "selected_width=8"                     \
     -v "fill_alpha=0.1";
