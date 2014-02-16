# vim:ts=4:sts=4:sw=4:tw=80:expandtab

function coefficient(nn, dd) {
    return int((nn % (stride ^ dd))) / (stride ^ dd);
}

function class2rgb(nn, aa) {
    aa["R"] = 256 * coefficient(nn, 1);
    aa["G"] = 256 * coefficient(nn, 2);
    aa["B"] = 256 * coefficient(nn, 3);
}

function class2hsl(nn, aa) {
    aa["H"] = 360 * coefficient(nn, 1);
    aa["S"] =       coefficient(nn, 2);
    aa["L"] =       coefficient(nn, 3);
}

function class2hsv(nn, aa) {
    aa["H"] = 360 * coefficient(nn, 1);
    aa["S"] =       coefficient(nn, 2);
    aa["V"] =       coefficient(nn, 3);
}

function read_RGB_index(filename) {
    ncolours = 0;
    delete RGB_index;
    while (getline < filename) {
        if ($0 ~ /^#/) {
            continue;
        } else if ($0 ~ /^[0-9]+\t[0-9]+\t[0-9]+\t/) {
            RGB_index[ncolours, "R"] = $1;
            RGB_index[ncolours, "G"] = $2;
            RGB_index[ncolours, "B"] = $3;
            ++ncolours;
        } else {
            info("Unrecognized line: " $0);
            exit(1);
        }
    }
}

function class2indexed_rgb(ii, aa) {
    aa["R"] = RGB_index[(ii % ncolours), "R"];
    aa["R"] = RGB_index[(ii % ncolours), "G"];
    aa["B"] = RGB_index[(ii % ncolours), "B"];
}

# SOURCE: http://www.rapidtables.com/convert/color/hsl-to-rgb.htm
# SOURCE: http://en.wikipedia.org/wiki/HSL_and_HSV#From_HSL
function hsl2rgb(aa,        cc, xx, mm, zz, tt, rr, gg, bb) {
    # hue        ∊ [0, 360)
    # saturation ∊ [0,   1]
    # lightness  ∊ [0,   1]
    # red        ∊ [0, 256) ∩ ℕ
    # green      ∊ [0, 256) ∩ ℕ
    # blue       ∊ [0, 256) ∩ ℕ
    cc = (1 - abs((2 * aa["L"]) - 1)) * aa["S"];
    xx = cc * (1 - abs(((aa["H"] / 60) % 2) - 1));
    mm = (aa["L"] - (cc / 2)) / 2;
    zz = int(aa["H"] / 60);
    tt[0] = tt[1] =  0;
    tt[2] = tt[5] = xx;
    tt[3] = tt[4] = cc;
    bb = tt[zz];
    gg = tt[(zz + 2) % 6];
    rr = tt[(zz + 4) % 6];

    aa["R"] = int(255 * (rr + mm));
    aa["G"] = int(255 * (gg + mm));
    aa["B"] = int(255 * (bb + mm));
}

# SOURCE: http://www.rapidtables.com/convert/color/hsv-to-rgb.htm
function hsv2rgb(aa,        cc, xx, mm, zz, tt, rr, gg, bb) {
    # hue        ∊ [0, 360)
    # saturation ∊ [0,   1]
    # value      ∊ [0,   1]
    # red        ∊ [0, 256) ∩ ℕ
    # green      ∊ [0, 256) ∩ ℕ
    # blue       ∊ [0, 256) ∩ ℕ
    cc = aa["V"] * aa["S"];
    xx = cc * (1 - abs(((aa["H"] / 60) % 2) - 1));
    mm = (aa["V"] - cc) / 2;
    zz = int(aa["H"] / 60);
    tt[0] = tt[1] =  0;
    tt[2] = tt[5] = xx;
    tt[3] = tt[4] = cc;
    bb = tt[zz];
    gg = tt[(zz + 2) % 6];
    rr = tt[(zz + 4) % 6];

    aa["R"] = int(255 * (rr + mm));
    aa["G"] = int(255 * (gg + mm));
    aa["B"] = int(255 * (bb + mm));
}

function rgba2hex(aa, alpha) {
    return sprintf("%02x%02x%02x%02x", int(255 * alpha),
                   aa["R"], aa["G"], aa["B"]);
}

function generate_filters(        ii) {
    xml_open_tag("annoFilters");
    for (ii = 0; ii < nclasses; ++ii) {
        xml_open_tag("annoFilter", "name", ("visualize class '" class[ii] "'"));
        xml_open_tag("filterRule");
        xml_open_and_close_tag("hasClass", "name", class[ii]);
        xml_close_tag("filterRule");
        xml_close_tag("annoFilter");
    }
    xml_close_tag("annoFilters");
}

function generate_colour_rules(        ii, hh, ss, ll, aa) {
    xml_open_tag("annoColorRules");
    for (ii = 0; ii < nclasses; ++ii) {
        if        (colour_method == "HSL_space") {
            class2hsl(ii, aa);
            hsl2rgb(aa);
        } else if (colour_method == "HSV_space") {
            class2hsv(ii, aa);
            hsv2rgb(aa);
        } else if (colour_method == "RGB_space") {
            class2rgb(ii, aa);
        } else if (colour_method == "RGB_index") {
            class2indexed_rgb(ii, aa);
        } else if (colour_method = "") {
            info("Undefined colour method");
            exit(1);
        } else {
            info("Unknown colour method: " colour_method);
            exit(1);
        }

        xml_open_tag("colorRule", "filterName", ("visualize class '" class[ii] "'"));

        xml_open_tag("visualShapeConfig");

        xml_open_tag("normalState");
        xml_text_element("borderWidth", normal_width);
        xml_text_element("borderColor", rgba2hex(aa, 1));
        xml_text_element("fillColor",   rgba2hex(aa, fill_alpha));
        xml_close_tag("normalState");

        xml_open_tag("selectedState");
        xml_text_element("borderWidth", selected_width);
        xml_text_element("borderColor", rgba2hex(aa, 1));
        xml_text_element("fillColor",   rgba2hex(aa, fill_alpha));
        xml_close_tag("selectedState");

        xml_close_tag("visualShapeConfig");

        xml_close_tag("colorRule");
    }
    xml_close_tag("annoColorRules");
}

BEGIN {
    nclasses = 0;
    delete class;
}
/<classDef id="[^"]+"/ {
    class[nclasses++] = gensub(/.*<classDef id="([^"]+)".*/, "\\1", 1);
}

BEGIN {
    if (colour_method == "RGB_index") {
        read_RGB_index(RGB_index_filename);
    }
}

END {
    stride = nclasses ** (1/3);
    info("nclasses       = " nclasses);
    info("stride         = " stride);
    info("colour method  = " colour_method);
    info("RGB index      = " RGB_index_filename);
    info("normal width   = " normal_width);
    info("selected width = " selected_width);
    info("flll alpha     = " fill_alpha);
    generate_filters();
    generate_colour_rules();
}
