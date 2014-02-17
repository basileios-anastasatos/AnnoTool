# vim:ts=4:sts=4:sw=4:tw=80:expandtab

BEGIN {
    min_score = "+inf";
    max_score = "-inf";
}
function process_rectangle(        aa, xx, yy, ww, hh, score, ss, nrect) {
    match($0, /^\(([-0-9.]+),([-0-9.]+),([-0-9.]+),([-0-9.]+)\)(:([-0-9.]+))?[,;.]$/, aa);

    xx = aa[1] + 0;
    yy = aa[2] + 0;
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
        nrect = nrectangles[image]++;
        rect2idx[ss] = nrect;
        rectangle[image, nrect, "xx"] = xx;
        rectangle[image, nrect, "yy"] = yy;
        rectangle[image, nrect, "ww"] = ww;
        rectangle[image, nrect, "hh"] = hh;
    } else {
        nrect = rect2idx[ss];
    }
    status[image, nrect] = (status[image, nrect] mode);
    if ((score != "null") && ((mode == "G") || (mode == "D"))) {
        scores[image, nrect, mode] = score;
        min_score = min(min_score, score);
        max_score = max(max_score, score);
    }
}

BEGIN {
    delete status2class;
    status2class["G"]   = "ground truth";
    status2class["DT"]  = \
    status2class["GDT"] = "true positive";
    status2class["DF"]  = "false positive";
    status2class["GM"]  = "missing recall";
}
function get_class_name(image, rect,        ss) {
    ss = status[image, rect];
    if (!(ss in status2class)) {
        fatal("Illegal status: " ss);
    } else {
        return status2class[ss];
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
    register_class("ground truth",   "0000ff"); # blue
    register_class("true positive",  "008000"); # green
    register_class("false positive", "ff0000"); # red
    register_class("missing recall", "ffff00"); # yellow
}

function generate_filters(        ii, jj, ff) {
    xml_open_tag("annoFilters");
    for (ii = 0; ii < nclasses; ++ii) {
        if ((class[ii] != "ground truth") && (filter_score == 1)) {
            for (jj = 0 ; jj < nscore_classes; ++jj) {
                ff = sprintf("%s %s", class[ii], score_class[jj]);
                xml_open_tag("annoFilter", "name", ff);
                xml_open_tag("filterRule");
                xml_open_tag("and");
                xml_open_and_close_tag("hasClass", "name", class[ii]);
                xml_open_and_close_tag("scoreCmp", "value",
                     score_class_left[jj], "op", "gte");
                xml_open_and_close_tag("scoreCmp", "value",
                     score_class_left[jj + 1], "op",
                     (jj + 1 == nscore_classes) ? "lte" : "lt");
                xml_close_tag("and");
                xml_close_tag("filterRule");
                xml_close_tag("annoFilter");
            }
        } else {
            xml_open_tag("annoFilter", "name", class[ii]);
            xml_open_tag("filterRule");
            xml_open_and_close_tag("hasClass", "name", class[ii]);
            xml_close_tag("filterRule");
            xml_close_tag("annoFilter");
        }
    }
    xml_close_tag("annoFilters");
}

function rgba2hex(rgb, alpha) {
    return sprintf("%02x%s", int(alpha * 255), rgb);
}

function generate_colour_rule(filter_name, colour, base_width, selected_width) {
    xml_open_tag("colorRule", "filterName", filter_name);

    xml_open_tag("visualShapeConfig");

    xml_open_tag("normalState");
    xml_text_element("borderWidth", base_width);
    xml_text_element("borderColor", rgba2hex(colour, 1));
    xml_text_element("fillColor",   rgba2hex(colour, fill_alpha));
    xml_close_tag("normalState");

    xml_open_tag("selectedState");
    xml_text_element("borderWidth", selected_width);
    xml_text_element("borderColor", rgba2hex(colour, 1));
    xml_text_element("fillColor",   rgba2hex(colour, fill_alpha));
    xml_close_tag("selectedState");

    xml_close_tag("visualShapeConfig");

    xml_close_tag("colorRule");
}

function generate_colour_rules(        ii, jj, ff) {
    xml_open_tag("annoColorRules");
    for (ii = 0; ii < nclasses; ++ii) {
        if ((class[ii] != "ground truth") && (filter_score == 1)) {
            for (jj = 0; jj < nscore_classes; ++jj) {
                ff = sprintf("%s %s", class[ii], score_class[jj]);
                generate_colour_rule(ff, colours[class[ii]],
                                    (jj + 1), (jj + 1) * 2);
            }
        } else {
            generate_colour_rule(class[ii], colours[class[ii]],
                                 normal_width, selected_width);
        }
    }
    xml_close_tag("annoColorRules");
}

function get_score(image, rect, class,        mode) {
    if ((class == "ground truth") || (class == "missing recall")) {
        mode = "G";
    } else {
        mode = "D";
    }
    if ((image, rect, mode) in scores) {
        return scores[image, rect, mode];
    } else {
        return "null";
    }
}

function generate_ata(image,        ii, nrect, uuid, file, class, score) {
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
        class = get_class_name(image, ii);
        xml_open_and_close_tag("class", "id", class);
        xml_close_tag("annoClass");
        score = get_score(image, ii, class);
        if (score != "null") {
            xml_open_tag("attributeValues");
            xml_text_element("attribute", score, "name", "__score");
            xml_close_tag("attributeValues");
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

function generate_atc(        ii, jj) {
    xml_start(atc);
    xml_header();
    xml_open_tag("annoClassDefinitions");
    for (ii = 0; ii < nclasses; ++ii) {
        xml_open_and_close_tag("classDef", "id", class[ii]);
    }
    xml_close_tag("annoClassDefinitions");
    xml_end(atc);
}

function compute_score_classes(        ii, ll, rr) {
    score_span = max_score - min_score;
    score_class_left[0] = ll = min_score;
    for (ii = 0; ii + 1 < nscore_classes; ++ii) {
        rr = min_score + (ii + 1) / nscore_classes * score_span;
        score_class     [ii    ] = sprintf("[%g,%g)", ll, rr);
        score_class_left[ii + 1] = rr;
        ll = rr;
    }
    rr = max_score;
    score_class     [ii    ] = sprintf("[%g,%g]", ll, rr);
    score_class_left[ii + 1] = rr;
}

END {
    nfiles = 1 + 1 + nimages;
    current_file = 0;
}
function show_progress() {
    info("File " ++current_file "/" nfiles);
}

BEGIN {
    filter_score = 0;
}
function generate_project(        ii, jj, nfiles) {
    complex_uuid = get_uuid();

    info("min_score = " min_score);
    info("max_score = " max_score);
    if ((nscore_classes > 1) && ((min_score + 0) < (max_score + 0))) {
        filter_score = 1;
        compute_score_classes();
    }

    show_progress();
    generate_atp();

    show_progress();
    generate_atc();

    for (ii = 0; ii < nimages; ++ii) {
        show_progress();
        generate_ata(images[ii]);
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
        images[nimages++] = image;
        nrectangles[image] = 0;
    }
}

/^\([-0-9.]+,[-0-9.]+,[-0-9.]+,[-0-9.]+\)(:[-0-9.]+)?[,;.]$/ {
    process_rectangle();
}

END {
    generate_project();
}
