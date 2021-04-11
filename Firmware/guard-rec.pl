$/ = undef;
s/(\s+(?!while|if|switch|for)([a-zA-Z0-9_]+)\s*\([^\)]*\)[\s\n]*{)/\1 REC_SENTINEL(\2);/mg;
