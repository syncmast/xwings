.PHONY: clean All

All:
	@echo "----------Building project:[ system - Debug ]----------"
	@cd "source/system" && $(MAKE) -f  "system.mk"
clean:
	@echo "----------Cleaning project:[ system - Debug ]----------"
	@cd "source/system" && $(MAKE) -f  "system.mk" clean
