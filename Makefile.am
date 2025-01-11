export SANITIZE_FLAGS

all:
	$(MAKE) -C src all

check: clean all
	$(MAKE) -C tests check SANITIZE_FLAGS="$(SANITIZE_FLAGS)"

clean:
	$(MAKE) -C src clean
	$(MAKE) -C tests clean

.PHONY: all check clean

