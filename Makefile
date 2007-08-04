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
	src/PlugOutUpgrade \
	src/ProjectControl \
	src/ProjectSynchro \
	src/RoundtripBody

PLUGOUT_DIRS = genplugouts/html/cpp \
	genplugouts/gpro \
	genplugouts/import_rose \
	genplugouts/singleton/cpp \
	genplugouts/cpp_utilities \
	genplugouts/xmi \
	genplugouts/xmi2 \
	genplugouts/xmi2import \
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
	src/ProjectControl/projectControl \
	src/ProjectSynchro/projectSynchro \
	src/RoundtripBody/roundtrip_body \
	genplugouts/html/cpp/ghtml \
	genplugouts/gpro/gpro \
	genplugouts/import_rose/irose \
	genplugouts/cpp_utilities/cpp_util \
	genplugouts/xmi/gxmi \
	genplugouts/xmi2/gxmi2 \
	genplugouts/xmi2import/ixmi2 \
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
	for i in $(SRC_DIRS) $(PLUGOUT_DIRS); do if [ -d $$i ]; then (cd $$i ; $(MAKE) clean; rm -f Makefile; ) || exit 1 ; fi; done

install:
	mkdir -p "$(DESTDIR)$(BOUML_LIB)"
	mkdir -p "$(DESTDIR)$(BOUML_DIR)"
	if test -n "$(BOUML_ICONS_PREFIX_DIR)" ; \
	then \
		for i in 16 32 48 64; do \
			mkdir -p "$(DESTDIR)$(BOUML_ICONS_PREFIX_DIR)/$$i"x"$$i/apps"; \
			cp bouml.$$i.png "$(DESTDIR)$(BOUML_ICONS_PREFIX_DIR)/$$i"x"$$i/apps/bouml.png"; \
			cp projectControl.$$i.png "$(DESTDIR)$(BOUML_ICONS_PREFIX_DIR)/$$i"x"$$i/apps/projectControl.png"; \
			cp projectSynchro.$$i.png "$(DESTDIR)$(BOUML_ICONS_PREFIX_DIR)/$$i"x"$$i/apps/projectSynchro.png"; \
		done \
	fi
	for i in $(PROGS); do cp -p $$i "$(DESTDIR)$(BOUML_LIB)" ; done
	cd plugouts ; tar cf - empty genpro html rose singleton cpp_utilities xmi xmi2 xmi2import sm_generator usecase_wizard sort FileControl deploy GlobalChange | (cd $(DESTDIR)$(BOUML_LIB); tar xf -)
	echo "#!/bin/sh" >$(DESTDIR)$(BOUML_DIR)/bouml
	echo "PATH=$(BOUML_LIB):$$"PATH >>$(DESTDIR)$(BOUML_DIR)/bouml
	echo "exec $(BOUML_LIB)/bouml \"$$"@"\"" >>$(DESTDIR)$(BOUML_DIR)/bouml
	chmod +x "$(DESTDIR)$(BOUML_DIR)/bouml"
	cd $(DESTDIR)$(BOUML_DIR) ; rm -f projectControl ; ln -s $(BOUML_LIB)/projectControl
	cd $(DESTDIR)$(BOUML_DIR) ; rm -f projectSynchro ; ln -s $(BOUML_LIB)/projectSynchro
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
		echo "Categories=Development;" >> "$(DESTDIR)$(BOUML_DESKTOP_DIR)/bouml.desktop" ; \
		echo "[Desktop Entry]" > "$(DESTDIR)$(BOUML_DESKTOP_DIR)/projectControl.desktop" ; \
		echo "Encoding=UTF-8" >> "$(DESTDIR)$(BOUML_DESKTOP_DIR)/projectControl.desktop" ; \
		echo "Name=Project Control" >> "$(DESTDIR)$(BOUML_DESKTOP_DIR)/projectControl.desktop" ; \
		echo "Type=Application" >> "$(DESTDIR)$(BOUML_DESKTOP_DIR)/projectControl.desktop" ; \
		echo "Comment=Bouml project's files control" >> "$(DESTDIR)$(BOUML_DESKTOP_DIR)/projectControl.desktop" ; \
		echo "Exec=projectControl" >> "$(DESTDIR)$(BOUML_DESKTOP_DIR)/projectControl.desktop" ; \
		echo "TryExec=projectControl" >> "$(DESTDIR)$(BOUML_DESKTOP_DIR)/projectControl.desktop" ; \
		echo "Path=$(BOUML_LIB)" >> "$(DESTDIR)$(BOUML_DESKTOP_DIR)/projectControl.desktop" ; \
		echo "Icon=projectControl" >> "$(DESTDIR)$(BOUML_DESKTOP_DIR)/projectControl.desktop" ; \
		echo "Categories=Development;" >> "$(DESTDIR)$(BOUML_DESKTOP_DIR)/projectControl.desktop" ; \
		echo "[Desktop Entry]" > "$(DESTDIR)$(BOUML_DESKTOP_DIR)/projectSynchro.desktop" ; \
		echo "Encoding=UTF-8" >> "$(DESTDIR)$(BOUML_DESKTOP_DIR)/projectSynchro.desktop" ; \
		echo "Name=Project Synchro" >> "$(DESTDIR)$(BOUML_DESKTOP_DIR)/projectSynchro.desktop" ; \
		echo "Type=Application" >> "$(DESTDIR)$(BOUML_DESKTOP_DIR)/projectSynchro.desktop" ; \
		echo "Comment=Bouml project's files synchronization" >> "$(DESTDIR)$(BOUML_DESKTOP_DIR)/projectSynchro.desktop" ; \
		echo "Exec=projectSynchro" >> "$(DESTDIR)$(BOUML_DESKTOP_DIR)/projectSynchro.desktop" ; \
		echo "TryExec=projectSynchro" >> "$(DESTDIR)$(BOUML_DESKTOP_DIR)/projectSynchro.desktop" ; \
		echo "Path=$(BOUML_LIB)" >> "$(DESTDIR)$(BOUML_DESKTOP_DIR)/projectSynchro.desktop" ; \
		echo "Icon=projectSynchro" >> "$(DESTDIR)$(BOUML_DESKTOP_DIR)/projectSynchro.desktop" ; \
		echo "Categories=Development;" >> "$(DESTDIR)$(BOUML_DESKTOP_DIR)/projectSynchro.desktop" ; \
	fi

uninstall:
	rm -rf $(DESTDIR)$(BOUML_LIB) $(DESTDIR)$(BOUML_DIR)/bouml
	rm -rf $(DESTDIR)$(BOUML_DIR)/projectControl $(DESTDIR)$(BOUML_DIR)/projectSynchro
	if test -n "$(BOUML_ICONS_PREFIX_DIR)" ; \
	then \
		for i in 16 32 48 64; do \
			rm "$(DESTDIR)$(BOUML_ICONS_PREFIX_DIR)/$$i"x"$$i/apps/bouml.png"; \
			rm "$(DESTDIR)$(BOUML_ICONS_PREFIX_DIR)/$$i"x"$$i/apps/projectControl.png"; \
			rm "$(DESTDIR)$(BOUML_ICONS_PREFIX_DIR)/$$i"x"$$i/apps/projectSynchro.png"; \
		done \
	fi
	if test -n "$(BOUML_DESKTOP_DIR)" ; \
	then \
		rm "$(DESTDIR)$(BOUML_DESKTOP_DIR)/bouml.desktop" ; \
		rm "$(DESTDIR)$(BOUML_DESKTOP_DIR)/projectControl.desktop" ; \
		rm "$(DESTDIR)$(BOUML_DESKTOP_DIR)/projectSynchro.desktop" ; \
	fi
