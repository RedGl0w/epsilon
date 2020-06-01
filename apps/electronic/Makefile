apps += Electronic::App
app_headers += apps/electronic/app.h

app_electronic_src = $(addprefix apps/electronic/,\
  app.cpp \
  menu_controller.cpp \
)

electronic_law = $(addprefix apps/electronic/,\
  ohm \
)

$(foreach i,${electronic_law},$(eval include $(i)/Makefile))

apps_src += $(app_electronic_src)

i18n_files += $(addprefix apps/electronic/,\
  base.en.i18n\
)


$(eval $(call depends_on_image,apps/electronic/app.cpp,apps/electronic/electronic_icon.png))
