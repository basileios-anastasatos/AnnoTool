# vim:ts=4:sts=4:sw=4:tw=80:expandtab

function generate_atp() {
    xml_header();
    xml_open_tag("annoProject", "name", project_name,
                                "uuid", complex_uuid);
    xml_open_tag("classPath");
    xml_text_element("file", "classes.atc");
    xml_close_tag("classPath");
    xml_open_tag("searchPath");
    xml_text_element("dir", "annotations");
    xml_close_tag("searchPath");
    xml_close_tag("annoProject");
}

BEGIN {
    generate_atp();
    exit(0);
}
