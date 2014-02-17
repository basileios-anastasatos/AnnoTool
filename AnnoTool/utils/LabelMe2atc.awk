# vim:ts=4:sts=4:sw=4:tw=80:expandtab

/<name>/ {
    class_name = xml_extract_text("name");
    if (! (class_name in name_set)) {
        ++nobj;
        name[nobj] = class_name;
        name_set[class_name] = "";
    }
}

function generate_atc(        ii, jj) {
    asort(name);
    xml_header();
    xml_open_tag("annoClassDefinitions");
    for (ii = 1; ii <= nobj; ++ii) {
        xml_open_and_close_tag("classDef", "id", name[ii]);
    }
    xml_close_tag("annoClassDefinitions");
}

END {
    generate_atc();
}
