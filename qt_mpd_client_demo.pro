TEMPLATE = subdirs

SUBDIRS += \
    Controller \
    src \
    tests

src.depends = Controller
tests.depends = Controller
