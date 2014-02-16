# vim:ts=4:sts=4:sw=4:tw=80:expandtab

function process_rectangle(        aa, xx, yy, ww, hh, score, ss, nrect) {
    match($0, /^\(([-0-9.]+),([-0-9.]+),([-0-9.]+),([-0-9.]+)\)(:([-0-9.]+))?[,;.]$/, aa);

    xx = 1.0 * aa[1];
    yy = 1.0 * aa[2];
    ww = aa[3] - xx;
    hh = aa[4] - yy;

    if (6 in aa) {
        score = aa[6];
    } else {
        score = "null";
    }

    ss = (image ":" xx "," yy "," ww "," hh);
    if (mode == "G") {
        assert(! (ss in rect2idx), ("! " ss " in rect2idx"));
    }
    if ((mode == "T") || (mode == "F") || (mode == "M")) {
        assert((ss in rect2idx), (ss " in rect2idx"));
    }
    if (! (ss in rect2idx)) {
        assert((mode == "G") || (mode == "D"), "mode ∊ {G, D}");
        nrect = nrectangles[image];
        rect2idx[ss] = nrect;
        rectangle[image, nrect, "xx"] = xx;
        rectangle[image, nrect, "yy"] = yy;
        rectangle[image, nrect, "ww"] = ww;
        rectangle[image, nrect, "hh"] = hh;
        status[image, nrect] = (status[image, nrect] mode);
        ++nrectangles[image];
    } else {
        nrect = rect2idx[ss];
        status[image, nrect] = (status[image, nrect] mode);
    }
    if ((mode == "D") && (score != "null")) {
        scores[image, nrect] = score;
    }
}

BEGIN {
    delete status2class;
    status2class["G"]   = "ground truth";
    status2class["DT"]  = "true positive";
    status2class["GDT"] = "ground truth and true positive";
    status2class["DF"]  = "false positive";
    status2class["GM"]  = "missing recall";
}
function get_class_name(image, ii,        ss) {
    ss = status[image, ii];
    if (ss in status2class) {
        return status2class[ss];
    } else {
        fatal("Illegal status: " ss);
    }
}

function register_class(name, colour) {
    class[nclasses++] = name;
    colours[name] = colour;
}

BEGIN {
    nclasses = 0;
    delete class;
    delete colours;
    register_class("ground truth",                   "0000ff"); # blue
    register_class("true positive",                  "008000"); # green
    register_class("ground truth and true positive", "ffc0cb"); # pink
    register_class("false positive",                 "ff0000"); # red
    register_class("missing recall",                 "ffff00"); # yellow
}

function generate_filters(        ii) {
    xml_open_tag("annoFilters");
    for (ii = 0; ii < nclasses; ++ii) {
        xml_open_tag("annoFilter", "name", class[ii]);
        xml_open_tag("filterRule");
        xml_open_and_close_tag("hasClass", "name", class[ii]);
        xml_close_tag("filterRule");
        xml_close_tag("annoFilter");
    }
    xml_close_tag("annoFilters");
}

function rgba2hex(rgb, alpha) {
    return sprintf("%02x%s", int(alpha * 255), rgb);
}

function generate_colour_rules(        ii, hh, ss, ll, aa) {
    xml_open_tag("annoColorRules");
    for (ii = 0; ii < nclasses; ++ii) {
        xml_open_tag("colorRule", "filterName", class[ii]);

        xml_open_tag("visualShapeConfig");

        xml_open_tag("normalState");
        xml_text_element("borderWidth", normal_width);
        xml_text_element("borderColor", rgba2hex(colours[class[ii]], 1));
        xml_text_element("fillColor",   rgba2hex(colours[class[ii]], fill_alpha));
        xml_close_tag("normalState");

        xml_open_tag("selectedState");
        xml_text_element("borderWidth", selected_width);
        xml_text_element("borderColor", rgba2hex(colours[class[ii]], 1));
        xml_text_element("fillColor",   rgba2hex(colours[class[ii]], fill_alpha));
        xml_close_tag("selectedState");

        xml_close_tag("visualShapeConfig");

        xml_close_tag("colorRule");
    }
    xml_close_tag("annoColorRules");
}

function generate_annotation_file(image,        ii, nrect, uuid, file) {
    uuid = get_uuid();
    file = (annotations_dir "/annotations_{" uuid "}.ata");
    xml_start(file);
    xml_header();
    xml_open_tag("imageAnnotationFile");
    xml_open_tag("imageInfo");
    xml_text_element("file", image);
    xml_text_element("uuid", uuid);
    xml_open_and_close_tag("source");
    xml_open_and_close_tag("comment");
    xml_close_tag("imageInfo");
    xml_open_tag("annotationInfo");
    xml_text_element("annotationComplex", complex_uuid);
    xml_close_tag("annotationInfo");
    xml_open_tag("imageAnnotations");
    nrect = nrectangles[image];
    for (ii = 0; ii < nrect; ++ii) {
        xml_open_tag("annotation", "uuid", get_uuid());
        xml_open_tag("annoClass");
        xml_open_and_close_tag("class", "id", get_class_name(image, ii));
        xml_close_tag("annoClass");
        if ((image, ii) in scores) {
            xml_open_and_close_tag("attributeValues", "__score",
                                                      scores[image, ii]);
        } else {
            xml_open_and_close_tag("attributeValues");
        }
        xml_open_tag("shape");
        xml_open_tag("rect");
        xml_open_and_close_tag("point", "x", rectangle[image, ii, "xx"],
                                        "y", rectangle[image, ii, "yy"]);
        xml_open_and_close_tag("size",  "w", rectangle[image, ii, "ww"],
                                        "h", rectangle[image, ii, "hh"]);
        xml_close_tag("rect");
        xml_close_tag("shape");
        xml_close_tag("annotation");
    }
    xml_close_tag("imageAnnotations");
    xml_close_tag("imageAnnotationFile");
    xml_end(file);
}

function generate_atp() {
    xml_start(atp);
    xml_header();
    xml_open_tag("annoProject", "name", atp, "uuid", complex_uuid);
    xml_open_tag("classPath");
    xml_text_element("file", atc);
    xml_close_tag("classPath");
    xml_open_tag("searchPath");
    xml_text_element("dir", annotations_dir);
    xml_close_tag("searchPath");
    generate_filters();
    generate_colour_rules();
    xml_close_tag("annoProject");
    xml_end(atp);
}

function generate_atc(        ii) {
    xml_start(atc);
    xml_header();
    xml_open_tag("annoClassDefinitions");
    for (ii = 0; ii < nclasses; ++ii) {
        xml_open_and_close_tag("classDef", "id", class[ii]);
    }
    xml_close_tag("annoClassDefinitions");
    xml_end(atc);
}

function generate_project(        ii, jj, nfiles) {
    nfiles = 1 + 1 + nimages;
    complex_uuid = get_uuid();
    info("File 1/" nfiles);
    generate_atp();
    info("File 2/" nfiles);
    generate_atc();
    for (ii = 0; ii < nimages; ++ii) {
        info("File " (ii + 3) "/" nfiles);
        generate_annotation_file(images[ii]);
    }
}

BEGIN {
    nimages = 0;
    delete images;
    delete nrectangles;
    delete rectangle;
    delete rect2idx;
    delete scores;
}

/^DATA [GDTFM]$/ {
    mode = $2;
}


/^"[^"]+"[:;]$/ {
    image = gensub(/^"([^"]+)":$/, "\\1", 1);
    if (mode == "G") {
        images[nimages] = image;
        nrectangles[image] = 0;
        ++nimages;
    }
}

/^\([-0-9.]+,[-0-9.]+,[-0-9.]+,[-0-9.]+\)(:[-0-9.]+)?[,;.]$/ {
    process_rectangle();
}

END {
    generate_project();
}
