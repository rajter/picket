CXX ?= g++

PROGRAM = picket
DESKTOP = $(PROGRAM).desktop
ICON = $(PROGRAM).svg
GLADES = $(wildcard ui/*.glade)

TARGET_EXE ?= picket

PREFIX ?= /usr

BUILD_DIR ?= ./build
SRC_DIRS ?= ./src

MKDIR_P ?= mkdir -p

SRCS := $(shell find $(SRC_DIRS) -name *.cpp -or -name *.c)
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

INC_DIRS := $(shell find $(SRC_DIRS) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

GTK_LIB = `pkg-config --libs gtkmm-3.0 --libs x11`
GTK_FLAG = `pkg-config --cflags gtkmm-3.0`
WARNING_FLAGS = -Wall -Wextra -Wconversion -Wno-unused-parameter
DEBUG_FLAG = -g

CPPFLAGS ?= $(INC_FLAGS) $(GTK_FLAG)

CXXFLAGS = $(GTK_FLAG) $(WARNING_FLAGS)

# first recipe: executable
$(TARGET_EXE): $(OBJS)
	$(CXX) $(OBJS) -o $@ $(LDFLAGS) $(GTK_LIB)

# c source
$(BUILD_DIR)/%.c.o: %.c
	$(MKDIR_P) $(dir $@)
	$(CXX) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

# c++ source
$(BUILD_DIR)/%.cpp.o: %.cpp
	$(MKDIR_P) $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@


debug: CXXFLAGS += $(DEBUG_FLAG)
debug: clean $(TARGET_EXE)

.PHONY: clean

clean:
	$(RM) -r $(BUILD_DIR)

.PHONY: install
install: $(PROGRAM)
	install -d $(DESTDIR)$(PREFIX)/share/$(PROGRAM)
	install -d $(DESTDIR)$(PREFIX)/share/applications
	install -d $(DESTDIR)$(PREFIX)/share/pixmaps
	install -d $(DESTDIR)$(PREFIX)/share/licenses/$(PROGRAM)

	install -D -m 0755 $(PROGRAM)           $(DESTDIR)$(PREFIX)/bin/$(PROGRAM)
	install -D -m 0644 $(GLADES)            $(DESTDIR)$(PREFIX)/share/$(PROGRAM)/
	install -D -m 0644 resources/$(DESKTOP) $(DESTDIR)$(PREFIX)/share/applications/
	install -D -m 0644 resources/$(ICON)    $(DESTDIR)$(PREFIX)/share/pixmaps/
	install -D -m 0644 LICENSE              $(DESTDIR)$(PREFIX)/share/licenses/$(PROGRAM)

.PHONY: uninstall
uninstall: $(PROGRAM)
	rm -f  $(DESTDIR)$(PREFIX)/bin/$(PROGRAM)
	rm -fr $(DESTDIR)$(PREFIX)/share/$(PROGRAM)
	rm -f  $(DESTDIR)$(PREFIX)/share/applications/$(DESKTOP)
	rm -f  $(DESTDIR)$(PREFIX)/share/pixmaps/$(ICON)
	rm -fr  $(DESTDIR)$(PREFIX)/share/licenses/$(PROGRAM)

-include $(DEPS)