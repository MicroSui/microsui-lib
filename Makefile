EXAMPLES_DIR := examples/*_examples
EXAMPLE_DIRS := $(wildcard $(EXAMPLES_DIR)/*)
OUTPUTS :=

.PHONY: all clean

all:
	@echo "Building all examples..."
	@for dir in $(EXAMPLE_DIRS); do \
		if [ -f $$dir/Makefile ]; then \
			echo "→ Building example in $$dir..."; \
			$(MAKE) -C $$dir || exit 1; \
		fi \
	done
	@echo ""
	@echo "✅ All examples built successfully!"
	@echo "---------------------------------------"
	@for dir in $(EXAMPLE_DIRS); do \
		if [ -f $$dir/Makefile ]; then \
			binary=$$(grep OUTPUT $$dir/Makefile | head -n1 | cut -d= -f2 | tr -d ' '); \
			if [ -n "$$binary" ] && [ -f $$dir/$$binary ]; then \
				echo "✔ Built $$binary in $$dir"; \
			fi \
		fi \
	done
	@echo "---------------------------------------"
