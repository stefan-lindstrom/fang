
# real mans
real_mans = sf_cfg.3 sf_cgi.3 sf_file.3 sf_fmt.3 sf_mem.3 sf_mime.3 \
	sf_misc.3 sf_sbuf.3 sf_sed.3 sf_split.3 sf_svect.3 sf_time.3 strfunc.3

# aliases of strfunc.3
strfunc_mans = libstrfunc.3

# aliases of sf_svect.3
sf_svect_mans = slist.3 sinit.3 svect.3 sadd.3 sadd2.3 sadd_attach.3 sdel.3 \
	sins.3 simport.3 sfree.3 sclear.3 sarray.3 mkarray.3 charize.3 \
	sfind.3 scfind.3 find.3 cfind.3 count_values.3 copy_values.3 \
	free_values.3 sget.3 scget.3 scgetp.3 scopy.3

# aliases of sf_split.3
sf_split_mans = split.3 splitf.3 splitquotable.3 split_network.3 sjoin.3 join.3


# aliases of sf_cgi.3
sf_cgi_mans = param.3 param2.3 params.3 set_cookie.3 cookie.3 cookies.3 \
	html_quote.3 url_encode.3 url_decode.3 getlanguageprefs.3

# aliases of sf_mime.3
sf_mime_mans = base64_encode.3 base64_decode.3 quoted_printable_decode.3 \
	mime_word_decode.3

# aliases of sf_time.3
sf_time_mans = strtotime.3 timetostr.3

# aliases of sf_cfg.3
sf_cfg_mans = cfgread.3 cfgget.3 cfgget2.3

# aliases of sf_fmt.3
sf_fmt_mans = format_init.3 format_free.3 format_metarule.3 formatf.3 \
	format_lastsize.3 format_lastresult.3 format_detach.3

# aliases of sf_sbuf.3
sf_sbuf_mans = sbuf.3 sbuf_init.3 sbuf_free.3 sbuf_detach.3 sbuf_clear.3 \
	sbuf_zero.3 sbuf_extend.3 sbuf_add.3 sbuf_add2.3 sbuf_sprintf.3 \
	sbuf_fetch.3 sbuf_fgets.3

# aliases of sf_sed.3
sf_sed_mans = sed.3 sed_compile.3 sed_exec.3 sed_free.3

# aliases of sf_file.3
sf_file_mans = initfdpos.3 initFILEpos.3 adjfdpos.3 adjFILEpos.3

# aliases of sf_misc.3
sf_misc_mans = eq.3 ecq.3 countchar.3 countchar2.3 countstr.3 replace.3 \
	replace_detach.3 limittextwidth.3

# aliases of sf_mem.3
sf_mem_mans = strndup.3 sf_strdup.3 sf_malloc.3 sf_calloc.3 sf_realloc.3 \
	strfunc_ctl.3

# all of the aliases
aliased_mans = $(strfunc_mans) $(sf_svect_mans) $(sf_split_mans) \
	$(sf_cgi_mans) $(sf_mime_mans) $(sf_time_mans) $(sf_cfg_mans) \
	$(sf_fmt_mans) $(sf_sbuf_mans) $(sf_sed_mans) $(sf_file_mans) \
	$(sf_misc_mans) $(sf_mem_mans)

man_MANS = $(real_mans) $(aliased_mans)

# if anyone knows a way to compress these all 
# to a single rule -- let the author know
$(strfunc_mans): strfunc.3
	cp strfunc.3 $@

$(sf_svect_mans): sf_svect.3
	cp sf_svect.3 $@

$(sf_split_mans): sf_split.3
	cp sf_split.3 $@

$(sf_cgi_mans): sf_cgi.3
	cp sf_cgi.3 $@

$(sf_mime_mans): sf_mime.3
	cp sf_mime.3 $@

$(sf_time_mans): sf_time.3
	cp sf_time.3 $@

$(sf_cfg_mans): sf_cfg.3
	cp sf_cfg.3 $@

$(sf_fmt_mans): sf_fmt.3
	cp sf_fmt.3 $@

$(sf_sbuf_mans): sf_sbuf.3
	cp sf_sbuf.3 $@

$(sf_sed_mans): sf_sed.3
	cp sf_sed.3 $@

$(sf_file_mans): sf_file.3
	cp sf_file.3 $@

$(sf_misc_mans): sf_misc.3
	cp sf_misc.3 $@

$(sf_mem_mans): sf_mem.3
	cp sf_mem.3 $@

CLEANFILES = $(aliased_mans)

EXTRA_DIST = $(man_MANS)

# aliased mans should be ignored by CVS
all: .cvsignore
extra_cvsignore = Makefile
.cvsignore: Makefile.am
	echo $(extra_cvsignore) $(aliased_mans) | tr ' ' '\n'  > $@
