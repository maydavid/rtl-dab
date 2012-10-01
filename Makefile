DIRS = src src/dab_demod 

.PHONY: subdirs $(DIRS)

subdirs: $(DIRS)

$(DIRS): 
	$(MAKE) -C $@

src: src/dab_demod

.PHONY: clean
clean:
	@for dir in $(DIRS) ; \
	do \
	if [ -f $$dir/Makefile ]; then \
	(cd $$dir ; \
	$(MAKE) clean ; \
	cd ..) ; \
	fi \
	done

