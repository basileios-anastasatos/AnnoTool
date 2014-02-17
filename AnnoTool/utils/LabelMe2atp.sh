#! /bin/bash
# vim:ts=4:sts=4:sw=4:tw=80:expandtab

readonly DIR=$(dirname "$0");

function msg() {
    echo $@ > /dev/stderr;
}

function fatal() {
    msg $@;
    exit 1;
}

if [ \! "$(type -P uuid)" ] ; then
    fatal "Fatal: command 'uuid' not found";
elif [ $# -ne 3 ] ; then
    fatal "Syntax: $0 <project_name> <source_dir> <images_dir>";
elif [ \! -d "${2:?}" ] ; then
    fatal "Fatal: $2 is not a directory.";
elif [ \! -d "${3:?}" ] ; then
    fatal "Fatal: $3 is not a directory.";
fi;

readonly PROJECT_NAME="${1:?}";    shift;
readonly ANNOTATIONS_DIR="${1:?}"; shift;
readonly IMAGES_DIR="${1:?}";      shift;

[ -d "${ANNOTATIONS_DIR:?}" ] ||
fatal "${ANNOTATIONS_DIR:?} does not exist or is not a directory";

[ -d "${IMAGES_DIR:?}" ] ||
fatal "${IMAGES_DIR:?} does not exist or is not a directory";

mkdir -p "${PROJECT_NAME:?}/annotations";

readonly PROJECT_FILE="${PROJECT_NAME:?}/${PROJECT_NAME:?}.atp";
readonly COMPLEX_UUID=$(uuid);
msg "Creating project file ${PROJECT_FILE:?}";
gawk -f "${DIR:?}/library.awk"     \
     -f "${DIR:?}/xml_library.awk" \
     -f "${DIR:?}/LabelMe2atp.awk" \
     -v project_name="${PROJECT_NAME:?}" \
     -v complex_uuid="${COMPLEX_UUID:?}" \
     > "${PROJECT_FILE:?}";

readonly TMP=$(mktemp);
rm -f "${TMP:?}";

function cleanup() {
    rm "${TMP:?}";
}

trap cleanup 1 2 3 9 15;

for XML in "${ANNOTATIONS_DIR:?}/"*.xml; do
    msg "Processing ${XML:?}";
    FILE_UUID=$(uuid);
    cat "${XML:?}"       |
    xargs echo           |
    sed -e "s///g"     \
        -e "s//'/g"  |
    xmllint --format -   |
    tee -a "${TMP:?}"    |
    gawk -f "${DIR:?}/library.awk"           \
         -f "${DIR:?}/xml_library.awk"       \
         -f "${DIR:?}/LabelMe2ata.awk"       \
         -v file_uuid="${FILE_UUID:?}"       \
         -v complex_uuid="${COMPLEX_UUID:?}" \
         -v images_dir="${IMAGES_DIR:?}"     \
         > "${PROJECT_NAME:?}/annotations/annotations_{${FILE_UUID:?}}.ata";
done;

readonly CLASS_FILE="${PROJECT_NAME:?}/classes.atc";
msg  "Creating class file" ${CLASS_FILE:?};
gawk -f "${DIR:?}/library.awk"     \
     -f "${DIR:?}/xml_library.awk" \
     -f "${DIR:?}/LabelMe2atc.awk" \
     "${TMP:?}" \
     > "${CLASS_FILE:?}";
