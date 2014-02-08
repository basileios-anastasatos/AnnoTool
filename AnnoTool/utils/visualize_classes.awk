#! /usr/bin/gawk -f
# vim:ts=4:sts=4:sw=4:tw=80:expandtab

function info(msg) {
    print msg > "/dev/stderr";
}

function abs(nn) {
    return (nn >= 0) ? nn : -nn;
}

# SOURCE: http://www.rapidtables.com/convert/color/hsl-to-rgb.htm
# SOURCE: http://en.wikipedia.org/wiki/HSL_and_HSV#From_HSL
function hsla2rgba(aa,        cc, xx, mm, zz, tt, rr, gg, bb) {
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

function rgba2hex(aa) {
    return sprintf("%02x%02x%02x%02x", 255 * aa["A"], aa["R"], aa["G"], aa["B"]);
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

function annoFilters(        ii) {
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
function annoColorRules(        ii, hh, ss, ll, aa) {
    PI = 3.14159265;
    dd = sqrt(nclasses);
    xml_open_tag("annoColorRules");
    for (ii = 0; ii < nclasses; ++ii) {
        aa["H"] = 360 * int(ii / dd) * dd / nclasses;
        aa["S"] = (ii % dd) / dd;
        xml_open_tag("colorRule", "filterName", ("visualize " class[ii]));

        xml_open_tag("visualShapeConfig");

        xml_open_tag("normalState");
        xml_text_element("borderWidth", base_width);
        aa["L"] = aa["A"] = 5/6; hsla2rgba(aa);
        xml_text_element("borderColor", rgba2hex(aa));
        aa["L"] = aa["A"] = 4/6; hsla2rgba(aa);
        xml_text_element("fillColor", rgba2hex(aa));
        xml_close_tag("normalState");

        xml_open_tag("selectedState");
        xml_text_element("borderWidth", (base_width * 2));
        aa["L"] = aa["A"] = 7/8; hsla2rgba(aa);
        xml_text_element("borderColor", rgba2hex(aa));
        aa["L"] = aa["A"] = 6/8; hsla2rgba(aa);
        xml_text_element("fillColor", rgba2hex(aa));
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
    info("nclasses = " nclasses);
    annoFilters();
    annoColorRules();
}
