SUB_DIRS=test_code src

.PHONY: all clean make_sub_dirs clean_sub_dirs

all : make_sub_dirs
make_sub_dirs:
	for sub_dir in $(SUB_DIRS);		\
	do					\
		$(MAKE) -C $${sub_dir};		\
	done

clean : clean_sub_dirs
clean_sub_dirs:
	for sub_dir in $(SUB_DIRS);				\
	do							\
		$(MAKE) -C $${sub_dir} -f Makefile clean;	\
	done

