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

mkdir "${PROJECT_NAME:?}";
mkdir "${PROJECT_NAME:?}/annotations";

readonly PROJECT_FILE="${PROJECT_NAME:?}/${PROJECT_NAME:?}.atp";
readonly COMPLEX_UUID=$(uuid);
msg "Creating project file ${PROJECT_FILE:?}";
cat >                     "${PROJECT_FILE:?}" <<-END
	<?xml version="1.0" encoding="UTF-8"?>
	<annoProject name="${PROJECT_NAME:?}" uuid="${COMPLEX_UUID:?}">
	    <classPath>
	        <file>classes.atc</file>
	    </classPath>
	    <searchPath>
	        <dir>annotations</dir>
	    </searchPath>
	</annoProject>
END

readonly TMP=$(mktemp);
rm -f "${TMP:?}";

for XML in "${ANNOTATIONS_DIR:?}/"*.xml; do
    msg "Processing ${XML:?}";
    FILE_UUID=$(uuid);
    cat "${XML:?}"       |
    xargs echo           |
    sed -e "s///g"     \
        -e "s//'/g"  |
    xmllint --format -   |
    tee -a "${TMP:?}"    |
    "${DIR:?}/xml2ata.awk" \
        -v file_uuid="${FILE_UUID:?}" \
        -v complex_uuid="${COMPLEX_UUID:?}" \
        -v images_dir="${IMAGES_DIR:?}" \
        > "${PROJECT_NAME:?}/annotations/annotations_{${FILE_UUID:?}}.ata";
done;

readonly CLASS_FILE="${PROJECT_NAME:?}/classes.atc";
msg  "Creating class file"                        ${CLASS_FILE:?};
echo '<?xml version="1.0" encoding="UTF-8"?>' >> "${CLASS_FILE:?}";
echo '<annoClassDefinitions>'                 >> "${CLASS_FILE:?}";
cat "${TMP:?}" |
sed -n \
    -e 's@.*<name>[[:space:]]*\([^>]*[^[:space:]]\)[[:space:]]*</name>.*@    <classDef id="\1"/>@p' |
sort -u                        >> "${CLASS_FILE:?}";
echo '</annoClassDefinitions>' >> "${CLASS_FILE:?}";
rm "${TMP:?}";
