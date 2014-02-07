#! /usr/bin/gawk -f
# vim:ts=4:sts=4:sw=4:tw=80:expandtab

function info(msg) {
    print msg > "/dev/stderr";
}

function abs(nn) {
    return (nn >= 0) ? nn : -nn;
}

function coefficient(nn, dd) {
    return int((nn % (cube_edge ^ dd))) / (cube_edge ^ dd);
}

function class2hsl(nn, aa) {
    aa["H"] = 360 * coefficient(nn, 1);
    aa["S"] =       coefficient(nn, 2);
    aa["L"] =       coefficient(nn, 3);
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
    mm = aa["L"] - (cc / 2);
    zz = int(aa["H"] / 60);
    tt[0] = tt[1] =  0;
    tt[2] = tt[5] = xx;
    tt[3] = tt[4] = cc;
    bb = tt[zz];
    gg = tt[(zz + 2) % 6];
    rr = tt[(zz + 4) % 6];

    aa["R"] = int(255 * (rr + (mm / 2)));
    aa["G"] = int(255 * (gg + (mm / 2)));
    aa["B"] = int(255 * (bb + (mm / 2)));
}

function rgba2hex(aa, alpha) {
    return sprintf("%02x%02x%02x%02x", 255 * alpha, aa["R"], aa["G"], aa["B"]);
}

BEGIN { ntags = 0; delete tag_stack; }
function push(tag) {
    tag_stack[++ntags] = tag;
}
function pop(tag) {
    if (tag_stack[ntags] != tag) {
        info("XML mismatch error: " tag_stack[ntags] " != " tag);
        exit(1);
    }
    delete tag_stack[ntags--];
}

function iprint(ss,        ii) {
    for (ii = 0; ii < ntags; ++ii) {
        printf("    "); # 4 blanks
    }
    printf("%s\n", ss);
}

function xml_open_tag(tag, property, value) {
    push(tag);
    if (property != "") {
        iprint("<" tag " " property "=\"" value "\">");
    } else {
        iprint("<" tag ">");
    }
}

function xml_open_and_close_tag(tag, property, value) {
    push(tag);
    if (property != "") {
        iprint("<" tag " " property "=\"" value "\"/>");
    } else {
        iprint("<" tag "/>");
    }
    pop(tag);
}

function xml_close_tag(tag) {
    iprint("</" tag ">");
    pop(tag);
}

function xml_text_element(tag, text) {
    push(tag);
    iprint("<" tag ">" text "</" tag ">");
    pop(tag);
}

function generate_filters(        ii) {
    xml_open_tag("annoFilters");
    for (ii = 0; ii < nclasses; ++ii) {
        xml_open_tag("annoFilter", "name", ("visualize " class[ii]));
        xml_open_tag("filterRule");
        xml_open_and_close_tag("hasClass", "name", class[ii]);
        xml_close_tag("filterRule");
        xml_close_tag("annoFilter");
    }
    xml_close_tag("annoFilters");
}

BEGIN { base_width = 4; }
function generate_colour_rules(        ii, hh, ss, ll, aa) {
    xml_open_tag("annoColorRules");
    for (ii = 0; ii < nclasses; ++ii) {
        class2hsl(ii, aa);
        hsl2rgb(aa);

        xml_open_tag("colorRule", "filterName", ("visualize " class[ii]));

        xml_open_tag("visualShapeConfig");

        xml_open_tag("normalState");
        xml_text_element("borderWidth", base_width);
        xml_text_element("borderColor", rgba2hex(aa, 0.8));
        xml_text_element("fillColor",   rgba2hex(aa, 0.6));
        xml_close_tag("normalState");

        xml_open_tag("selectedState");
        xml_text_element("borderWidth", (base_width * 2));
        xml_text_element("borderColor", rgba2hex(aa, 0.9));
        xml_text_element("fillColor",   rgba2hex(aa, 0.7));
        xml_close_tag("selectedState");

        xml_close_tag("visualShapeConfig");

        xml_close_tag("colorRule");
    }
    xml_close_tag("annoColorRules");
}

BEGIN { nclasses = 0; }
/<classDef id="[^"]+"/ {
    class[nclasses++] = gensub(/.*<classDef id="([^"]+)".*/, "\\1", 1);
}

END {
    cube_edge =         nclasses ** (1/3);
    info("nclasses = "  nclasses);
    info("cube_edge = " cube_edge);
    generate_filters();
    generate_colour_rules();
}
