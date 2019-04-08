SET(CMAKE_INSTALL_PREFIX /$(CURDIR)/usr)
 # set project's name
  PROJECT( glpeces )
 
  cmake_minimum_required(VERSION 2.8)
  
  # with SET() command you can change variables or define new ones
  # here we define SAMPLE_SRCS variable that contains a list of all .cpp files
  # note that we don't need \ at the end of line
  SET( glpeces_SRCS
	./scr/main.cpp
       	./scr/configuracio.cpp 
       	./scr/funcions.cpp 
	./scr/rellotge.cpp 
	./scr/creaconcurs.cpp 
	./scr/desafigura.cpp 
	./scr/frmPrincipal.cpp 
	./scr/conjuntpeces.cpp
	./scr/peca.cpp 
	./scr/comprovafigures.cpp
  )
  
  # another list, this time it includes all header files that should be treated with moc
  SET( glpeces_MOC_HDRS       
     ./scr/configuracio.h
     ./scr/constants.h 
     ./scr/funcions.h 
     ./scr/rellotge.h 
     ./scr/creaconcurs.h 
     ./scr/desafigura.h 
     ./scr/ui_frmDesaFigura.h 
     ./scr/frmPrincipal.h
     ./scr/conjuntpeces.h 
     ./scr/peca.h 
     ./scr/comprovafigures.h     
  )
  
  # some .ui files
  SET( glpeces_UIS
       ./scr/frmDesaFigura.ui
  )
  
  # and finally an resource file
 # SET( glpeces_RCS
 #     resources.qrc
 # )
  
  # enable warnings
  ADD_DEFINITIONS( -Wall )
  
  # by default only QtCore and QtGui modules are enabled
  # other modules must be enabled like this:
  SET( QT_USE_QT3SUPPORT TRUE )   
  SET( QT_USE_QTXML TRUE )
  set(QT_USE_QTNETWORK true) 
  set(QT_USE_TESTLIB true)
  set(QT_USE_QTSVG true)
  set(QT_USE_QTDBUS true)
  
  # this command finds Qt4 libraries and sets all required variables
  # note that it's Qt4, not QT4 or qt4
  find_package(Qt5Widgets)
  #find_package(Qt5Declarative)

  # The Qt5Widgets_INCLUDES also includes the include directories for
  # dependencies QtCore and QtGui
  include_directories(${Qt5Widgets_INCLUDES})

  # We need add -DQT_WIDGETS_LIB when using QtWidgets in Qt 5.
  add_definitions(${Qt5Widgets_DEFINITIONS})
  
  # add some useful macros and variables
  # (QT_USE_FILE is a variable defined by FIND_PACKAGE( Qt4 ) that contains a path to CMake script)
  #INCLUDE( ${QT_USE_FILE} )
  
  # this command will generate rules that will run rcc on all files from glpeces_RCS
  # in result glpeces_RC_SRCS variable will contain paths to files produced by rcc
  #QT4_ADD_RESOURCES( glpeces_RC_SRCS ${glpeces_RCS} )
  
  # this will run uic on .ui files:
  QT5_WRAP_UI( glpeces_UI_HDRS ${glpeces_UIS} )
  
  # and finally this will run moc:
  QT5_WRAP_CPP( glpeces_MOC_SRCS ${glpeces_MOC_HDRS} )
  
  # we need this to be able to include headers produced by uic in our code
  # (CMAKE_BINARY_DIR holds a path to the build directory, while INCLUDE_DIRECTORIES() works just like INCLUDEPATH from qmake)
  INCLUDE_DIRECTORIES( ${CMAKE_BINARY_DIR} )
  
  # binaries are placed in the root directory as the source code
  SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${glpeces_SOURCE_DIR})
  # here we instruct CMake to build "glpeces" executable from all of the source files
  ADD_EXECUTABLE( glpeces ${glpeces_SRCS} ${glpeces_MOC_SRCS} ${glpeces_UI_HDRS} )
  
  # last thing we have to do is to tell CMake what libraries our executable needs,
  # luckily FIND_PACKAGE prepared QT_LIBRARIES variable for us:
  #TARGET_LINK_LIBRARIES( glpeces ${QT_LIBRARIES} 
  #			${QT_QTMAIN_LIBRARY}
  #			${QT_QTTEST_LIBRARY})

  target_link_libraries(glpeces ${Qt5Widgets_LIBRARIES})

  # install executable and translation files
install_targets(/games glpeces)
 # INSTALL( TARGETS ${PROGNAME} DESTINATION bin )
  install(DIRECTORY fig/ DESTINATION share/games/glpeces/figures)
  install(DIRECTORY locales/ DESTINATION share/games/glpeces/locales)
  install(DIRECTORY images/ DESTINATION share/games/glpeces/images)
  install(DIRECTORY help/ DESTINATION share/games/glpeces/help)
  install(DIRECTORY auto/ DESTINATION share/games/glpeces/auto)
  install(FILES menu/glpeces.6.gz DESTINATION share/man/man6)
  install(FILES menu/glpeces.desktop DESTINATION share/applications)
  install(FILES glpeces.xpm DESTINATION share/pixmaps)

 #uninstall
CONFIGURE_FILE( "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in" "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake" IMMEDIATE @ONLY)
ADD_CUSTOM_TARGET(uninstall "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake" )