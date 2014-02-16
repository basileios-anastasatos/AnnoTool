# vim:ts=4:sts=4:sw=4:tw=80:expandtab

BEGIN {
    ntags = 0;
    delete tag_stack;
}
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

BEGIN {
    ofile = "/dev/stdout";
}

function xml_start(file) {
    ofile = file;
}

function xml_end(file) {
    assert((file == ofile), (file " == " ofile));
    close(ofile);
}

function iprint(ss,        ii) {
    for (ii = 0; ii < ntags; ++ii) {
        printf("    ") > ofile; # 4 blanks
    }
    printf("%s\n", ss) > ofile;
}

function xml_header() {
    print "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" > ofile;
}

function xml_open_tag(tag, property0, value0, property1, value1) {
    push(tag);
    if (property1 != "") {
        iprint("<" tag " " property0 "=\"" value0 "\" " property1 "=\"" value1 "\">");
    } else if (property0 != "") {
        iprint("<" tag " " property0 "=\"" value0 "\">");
    } else {
        iprint("<" tag ">");
    }
}

function xml_open_and_close_tag(tag, property0, value0, property1, value1) {
    push(tag);
    if (property1 != "") {
        iprint("<" tag " " property0 "=\"" value0 "\" " property1 "=\"" value1 "\"/>");
    } else if (property0 != "") {
        iprint("<" tag " " property0 "=\"" value0 "\"/>");

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

function xml_extract_text(tag) {
    return trim(gensub(".*<" tag ">([^<]*)<\\/" tag ">.*", "\\1", "1"));
}
