# vim:ts=4:sts=4:sw=4:tw=80:expandtab

function info(msg) {
    print msg > "/dev/stderr";
}

END {
    if (ASSERTION_FAILED == 1) {
        exit(1);
    }
}
function assert(assertion, msg) {
    if (! assertion) {
        info("Assertion failed: " msg);
        ASSERTION_FAILED = 1;
        exit(1);
    }
}

BEGIN {
    if (FATAL == 1) {
        exit(1);
    }
}
function fatal(msg) {
    info(msg);
    FATAL = 1;
    exit(1);
}

function get_uuid(uuid) {
    "uuid" | getline uuid;
    close("uuid");
    return uuid;
}

function abs(nn) {
    return (nn >= 0) ? nn : -nn;
}

function trim(ss) {
    return gensub(/^[[:space:]]*/, "", "1",
           gensub(/[[:space:]]*$/, "", "1", ss));
}
