#! /usr/bin/gawk -f
# vim:ts=4:sts=4:sw=4:tw=80:expandtab

function trim(ss) {
    return gensub(/^[[:space:]]*/, "", "1",
           gensub(/[[:space:]]*$/, "", "1", ss));
}

function extract(tag) {
    return trim(gensub(".*<" tag ">([^<]*)<\\/" tag ">.*", "\\1", "1"));
}

/<filename>/ {
    filename = extract("filename");
}

/<name>/ {
    ++nobj;
    name[nobj] = extract("name");
    getline;
    ids[nobj] = extract("id");
}

/<pt>/ {
    ++npt[nobj];
    getline;
    ptx[nobj, npt[nobj]] = extract("x");
    getline;
    pty[nobj, npt[nobj]] = extract("y");
}

function get_uuid(uuid) {
    "uuid" | getline uuid;
    close("uuid");
    return uuid;
}

END {
    print "<?xml version=\"1.0\" encoding=\"UTF-8\"?>";
    print "<imageAnnotationFile>";
    print "    <imageInfo>";
    print "        <file>" images_dir "/" filename "</file>";
    print "        <uuid>" file_uuid "</uuid>";
    print "        <source/>";
    print "        <comment/>";
    print "    </imageInfo>";
    print "    <annotationInfo>";
    print "        <annotationComplex>" complex_uuid "</annotationComplex>";
    print "    </annotationInfo>";
    print "    <imageAnnotations>";
    for (ii = 1; ii <= nobj; ++ii) {
        print "        <annotation uuid=\"" get_uuid() "\">";
        print "            <annoClass>";
        print "                <class id=\"" name[ii] "\"/>";
        print "            </annoClass>";
        print "            <attributeValues/>";
        print "            <shape>";
        print "                <polygon>";
        for (jj = 1; jj <= npt[ii]; ++jj) {
            print "                    <point x=\"" ptx[ii, jj] \
                                          "\" y=\"" pty[ii, jj] "\"/>";
        }
        print "                </polygon>";
        print "            </shape>";
        print "        </annotation>";
    }
    print "    </imageAnnotations>";
    print "</imageAnnotationFile>";
}
