# vim:ts=4:sts=4:sw=4:tw=80:expandtab

/<filename>/ {
    filename = xml_extract_text("filename");
}

BEGIN {
    nobj = 0;
    delete name;
    delete ids;
    delete npt;
    delete ptx;
    delete pty;
}
/<name>/ {
    ++nobj;
    name[nobj] = xml_extract_text("name");
    getline;
    ids[nobj] = xml_extract_text("id");
}

/<pt>/ {
    ++npt[nobj];
    getline;
    ptx[nobj, npt[nobj]] = xml_extract_text("x");
    getline;
    pty[nobj, npt[nobj]] = xml_extract_text("y");
}

function generate_ata(        ii, jj) {
    xml_header();
    xml_open_tag("imageAnnotationFile");
    xml_open_tag("imageInfo");
    xml_text_element("file", (images_dir "/" filename));
    xml_text_element("uuid", file_uuid);
    xml_open_and_close_tag("source");
    xml_open_and_close_tag("comment");
    xml_close_tag("imageInfo");
    xml_open_tag("annotationInfo");
    xml_text_element("annotationComplex", complex_uuid);
    xml_close_tag("annotationInfo");
    xml_open_tag("imageAnnotations");
    for (ii = 1; ii <= nobj; ++ii) {
        xml_open_tag("annotation", "uuid", get_uuid());
        xml_open_tag("annoClass");
        xml_open_and_close_tag("class", "id", name[ii]);
        xml_close_tag("annoClass");
        xml_open_and_close_tag("attributeValues");
        xml_open_tag("shape");
        xml_open_tag("polygon");
        for (jj = 1; jj <= npt[ii]; ++jj) {
            xml_open_and_close_tag("point", "x", ptx[ii, jj],
                                            "y", pty[ii, jj]);
        }
        xml_close_tag("polygon");
        xml_close_tag("shape");
        xml_close_tag("annotation");
    }
    xml_close_tag("imageAnnotations");
    xml_close_tag("imageAnnotationFile");
}

END {
    generate_ata();
}
