# find make_define file

SUB_DIRS=queue thread

.PHONY: all clean make_sub_dirs clean_sub_dirs

all: make_sub_dirs
make_sub_dirs:
	for sub_dir in $(SUB_DIRS);				\
	do							\
		$(MAKE) -C $${sub_dir};				\
	done


clean: make_clean_dirs
make_clean_dirs:
	for sub_dir in $(SUB_DIRS);				\
	do							\
		$(MAKE) -C $${sub_dir} -f Makefile clean;	\
	done
