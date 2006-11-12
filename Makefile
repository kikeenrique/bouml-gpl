# The directory where all the bouml files will be installed, you
# can change it for instance because you don't have root permission
BOUML_LIB = /usr/lib/bouml

# The directory where the bouml shell script will be installed, you
# can change it for instance because you don't have root permission
BOUML_DIR = /usr/bin

# The directory containing bouml.desktop
# The copy is not done if you comment the definition
BOUML_DESKTOP_DIR = /usr/share/applications

# The directories containing the bouml icons are
# <BOUML_ICONS_PREFIX_DIR>/<size>x<size>/apps
# The copy is not done if you comment the definition
BOUML_ICONS_PREFIX_DIR = /usr/share/icons/hicolor

# for packaging purpose, useless by default
# DESTDIR = 

# uncomment if needed
# MAKE = make

QMAKE=qmake

SRC_DIRS = src \
	src/CppGenerator \
	src/CppReverse \
	src/JavaGenerator \
	src/JavaReverse \
	src/JavaCat \
	src/IdlGenerator \
	src/PlugOutUpgrade

PLUGOUT_DIRS = genplugouts/html/cpp \
	genplugouts/gpro \
	genplugouts/import_rose \
	genplugouts/singleton/cpp \
	genplugouts/cpp_utilities \
	genplugouts/xmi \
	genplugouts/sm/cpp \
	genplugouts/usecasewizard/cpp \
	genplugouts/sort/cpp \
	genplugouts/file_control \
	genplugouts/deploy/cpp \
	genplugouts/global_change

PROGS = src/bouml \
	src/CppGenerator/cpp_generator \
	src/CppReverse/cpp_reverse \
	src/JavaGenerator/java_generator \
	src/JavaReverse/java_reverse \
	src/JavaCat/java_catalog \
	src/IdlGenerator/idl_generator \
	src/PlugOutUpgrade/plug_out_upgrade \
	genplugouts/html/cpp/ghtml \
	genplugouts/gpro/gpro \
	genplugouts/import_rose/irose \
	genplugouts/cpp_utilities/cpp_util \
	genplugouts/xmi/gxmi \
	genplugouts/sm/cpp/stmgen \
	genplugouts/usecasewizard/cpp/usecasewizard \
	genplugouts/sort/cpp/browsersort \
	genplugouts/file_control/file_control \
	genplugouts/deploy/cpp/deplcl \
	genplugouts/global_change/global_change

compile:
	@echo Generate BOUML and the hand made plug-outs

	for i in $(SRC_DIRS); do if [ -d $$i ]; then ( cd $$i; $(QMAKE); ) || exit 1 ; $(MAKE) -C $$i || exit 1 ; fi; done

	@echo Generate plug-outs

	for i in $(PLUGOUT_DIRS); do if [ -d $$i ]; then ( cd $$i; $(QMAKE); ) || exit 1 ; $(MAKE) -C $$i || exit 1 ; fi; done

clean:
	for i in $(SRC_DIRS) $(PLUGOUT_DIRS); do if [ -d $$i ]; then cd $$i ; $(MAKE) clean || exit 1 ; fi; done

install:
	mkdir -p "$(DESTDIR)$(BOUML_LIB)"
	mkdir -p "$(DESTDIR)$(BOUML_DIR)"
	if test -n "$(BOUML_ICONS_PREFIX_DIR)" ; \
	then \
		for i in 16 32 48 64; do \
			mkdir -p "$(DESTDIR)$(BOUML_ICONS_PREFIX_DIR)/$$i"x"$$i/apps"; \
			cp bouml.$$i.png "$(DESTDIR)$(BOUML_ICONS_PREFIX_DIR)/$$i"x"$$i/apps/bouml.png"; \
		done \
	fi
	for i in $(PROGS); do cp -p $$i "$(DESTDIR)$(BOUML_LIB)" ; done
	cd plugouts ; tar cf - empty genpro html rose singleton cpp_utilities xmi sm_generator usecase_wizard sort FileControl deploy GlobalChange | (cd $(DESTDIR)$(BOUML_LIB); tar xf -)
	echo "#!/bin/sh" >$(DESTDIR)$(BOUML_DIR)/bouml
	echo "PATH=$(BOUML_LIB):$$"PATH >>$(DESTDIR)$(BOUML_DIR)/bouml
	echo "$(BOUML_LIB)/bouml \"$$"@"\"" >>$(DESTDIR)$(BOUML_DIR)/bouml
	chmod +x "$(DESTDIR)$(BOUML_DIR)/bouml"
	if test -n "$(BOUML_DESKTOP_DIR)" ; \
	then \
		mkdir -p "$(DESTDIR)$(BOUML_DESKTOP_DIR)" ; \
		echo "[Desktop Entry]" > "$(DESTDIR)$(BOUML_DESKTOP_DIR)/bouml.desktop" ; \
		echo "Encoding=UTF-8" >> "$(DESTDIR)$(BOUML_DESKTOP_DIR)/bouml.desktop" ; \
		echo "Name=BOUML" >> "$(DESTDIR)$(BOUML_DESKTOP_DIR)/bouml.desktop" ; \
		echo "Type=Application" >> "$(DESTDIR)$(BOUML_DESKTOP_DIR)/bouml.desktop" ; \
		echo "Comment=Free UML 2 modeler" >> "$(DESTDIR)$(BOUML_DESKTOP_DIR)/bouml.desktop" ; \
		echo "Exec=bouml" >> "$(DESTDIR)$(BOUML_DESKTOP_DIR)/bouml.desktop" ; \
		echo "TryExec=bouml" >> "$(DESTDIR)$(BOUML_DESKTOP_DIR)/bouml.desktop" ; \
		echo "Path=$(BOUML_LIB)" >> "$(DESTDIR)$(BOUML_DESKTOP_DIR)/bouml.desktop" ; \
		echo "Icon=bouml" >> "$(DESTDIR)$(BOUML_DESKTOP_DIR)/bouml.desktop" ; \
		echo "Categories=Application;Development;" >> "$(DESTDIR)$(BOUML_DESKTOP_DIR)/bouml.desktop" ; \
	fi

uninstall:
	rm -rf $(DESTDIR)$(BOUML_LIB) $(DESTDIR)$(BOUML_DIR)/bouml
