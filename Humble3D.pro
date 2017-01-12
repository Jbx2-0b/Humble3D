TEMPLATE = subdirs

unix:!mac: message("Using settings for Unix/Linux.")
android: message("Using settings for Android.")
mac: message("Using settings for MacOS.")
win32: message("Using settings for Windows.")


SUBDIRS += \
    lib_tools \
    lib_math \
    lib_xml \
    #lib_network \
    lib_core \
    test_lib_math \
    lib_assets \
    assimp \
    lib_assimp \
    lib_opengl \
    lib_qt_ui \
    lib_scripting \
    bullet \
    lib_physics \
    test_transformation \
    test_qml \
    test_animation \
    test_multipass \
    test_physics \
    test_software_renderer \
    test_octree \
    #test_tcp_server \
    #test_tcp_client \
    scriptloader \
    labo \
    formatconverter \
    frageditor

assimp.depends =
bullet.depends =

lib_assets.depends = lib_tools lib_math lib_core
lib_assimp.depends = lib_tools lib_math lib_core assimp
lib_core.depends = lib_tools lib_math
lib_math.depends =
lib_opengl.depends = lib_tools lib_math lib_core
lib_physics.depends = lib_tools lib_math lib_core lib_assets bullet
lib_qt_ui.depends = lib_tools lib_math lib_core lib_opengl
lib_tools.depends =
lib_scripting.depends = lib_tools lib_math lib_core lib_assimp assimp
lib_xml.depends =

test_animation.depends = lib_tools lib_math lib_core lib_opengl lib_assimp assimp lib_qt_ui
test_lib_math.depends = lib_tools lib_math lib_core lib_xml
test_multipass.depends = lib_tools lib_math lib_core lib_assets lib_opengl lib_qt_ui lib_assimp assimp
test_octree.depends = lib_tools lib_math lib_core lib_assets lib_opengl lib_qt_ui
test_physics.depends = lib_tools lib_math lib_core lib_assets lib_opengl lib_qt_ui lib_assets lib_assimp assimp lib_physics bullet
test_software_renderer.depends = lib_tools lib_math lib_core lib_opengl lib_qt_ui lib_assimp assimp
test_transformation.depends = lib_tools lib_math lib_core lib_assets lib_opengl lib_qt_ui
test_qml.depends = lib_tools lib_math lib_core lib_assets lib_opengl lib_qt_ui

scriptloader.depends = lib_tools lib_math lib_core lib_assets lib_opengl lib_qt_ui lib_scripting
labo.depends = lib_tools lib_math lib_core lib_assets lib_opengl lib_qt_ui lib_assimp assimp lib_scripting
formatconverter.depends = lib_tools lib_math lib_core lib_assimp assimp
frageditor.depends = lib_tools lib_math lib_core lib_assets lib_opengl lib_qt_ui

