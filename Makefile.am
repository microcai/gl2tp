EXTRA_DIST = m4 

SUBDIRS  = icons src doc po

test : $(top_builddir)/src/gl2tp /tmp/${prefix}/share/locale/
	$(top_builddir)/src/gl2tp 

/tmp/${prefix}/share/locale/ :
	cd po && $(MAKE) DESTDIR=/tmp install
	
$(top_builddir)/src/gl2tp :
	cd src && $(MAKE)
			