TOPTARGETS = all clean
SUBDIRS = src tests examples
GIT_HOOKS := .git/hooks/applied

.PHONY: $(TOPTARGETS) $(SUBDIRS) check
all: $(GIT_HOOK) $(SUBDIRS)

$(GIT_HOOK):
	@./scripts/install-git-hooks
	@echo

$(TOPTARGETS): $(SUBDIRS)
$(SUBDIRS):
	@$(MAKE) -C $@ $(MAKECMDGOALS)

check:
	@$(MAKE) -C tests check

demo:
	@$(MAKE) -C examples run
