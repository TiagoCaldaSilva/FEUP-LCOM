PROG=proj

SRCS = proj.c timer.c keyboard.c mouse.c video_card.c rtc.c sprite.c utils.c settings_menu.c canvas.c main_menu.c play_menu.c avatar.c playground_menu.c

# __LCOM_OPTIMIZED__ disables most of the LCF's overhead (and functionality!)
CPPFLAGS += -pedantic -D __LCOM_OPTIMIZED__

DPADD += ${LIBLCF}
LDADD += -llcf

.include <minix.lcom.mk>
