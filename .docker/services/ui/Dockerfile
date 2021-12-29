	FROM gcc:latest as builder
	RUN :												\
		&&	apt-get update								\
		&&	apt-get install -y --no-install-recommends	\
			meson ninja-build							\
			libgtk-3-dev								\
		&&	true										;
	
	#cspell:words libui
	WORKDIR			/usr/src/libui
	COPY	libui	.
	
	RUN :												\
		&&	meson setup --wipe build					\
		&&	ninja -C build								\
		&&	ninja -C build install						\
		&&	true										;

	FROM php:7-cli
	
	#cspell:words libgtk
	RUN :												\
		&& apt-get update								\
		&& apt-get install -y --no-install-recommends	\
			libgtk-3-dev								\
		&& true											;
#	RUN :	true										\
#		&&	apt-get update								\
#		&&	apt-get install -y --no-install-recommends	\
#			less										\
#			vim											\
#		&&	true										;
	
	COPY --from=builder	/usr/src/libui/build/meson-out/libui.so.0	/usr/local/lib/x86_64-linux-gnu/
	RUN ln -s										   libui.so.0	/usr/local/lib/x86_64-linux-gnu/libui.so
	COPY --from=builder	/usr/src/libui/build/meson-out/libui.so.0	/usr/local/lib/
	RUN ln -s										   libui.so.0	/usr/local/lib/libui.so
	COPY --from=builder	/usr/src/libui/ui_unix.h					/usr/local/include/
	COPY --from=builder	/usr/src/libui/ui.h							/usr/local/include/
	
	WORKDIR			/usr/src/ui
	COPY	ui		.
	
	#cspell:words phpize
	RUN :	true																														\
		&&	phpize																														\
		&&	./configure																													\
		&&	make clean																													\
		&&	make																														\
		&&	true																														;