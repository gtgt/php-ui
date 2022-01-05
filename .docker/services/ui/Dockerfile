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
	
	# failed attempt at colorizing meson/ninja - fuck it
	# even with: d-c down --rmi all
	ARG TERM
	ENV TERM=$TERM
	RUN echo TERM=$TERM
	
	RUN :												\
		&&	meson setup --wipe build					\
		&&	ninja -v -C build							\
		&&	ninja -v -C build install					\
		&&	true										;

	FROM php:7-cli
	
	#cspell:words libgtk
	RUN :												\
		&& apt-get update								\
		&& apt-get install -y --no-install-recommends	\
			libgtk-3-dev								\
		&& true											;
	
	# Optional simple tools
	RUN :	true										\
		&&	apt-get update								\
		&&	apt-get install -y --no-install-recommends	\
			less										\
			vim											\
		&&	true										;
	
	# Optional complex tools
	RUN :	true										\
		&&	apt-get update								\
		&&	apt-get install -y --no-install-recommends	\
			gdb											\
		&&	true										;
	
	COPY --from=builder	/usr/src/libui/build/meson-out/libui.so.0	/usr/local/lib/x86_64-linux-gnu/
	RUN ln -s										   libui.so.0	/usr/local/lib/x86_64-linux-gnu/libui.so
	COPY --from=builder	/usr/src/libui/build/meson-out/libui.so.0	/usr/local/lib/
	RUN ln -s										   libui.so.0	/usr/local/lib/libui.so
	COPY --from=builder	/usr/src/libui/ui_unix.h					/usr/local/include/
	COPY --from=builder	/usr/src/libui/ui.h							/usr/local/include/
	
	WORKDIR			/usr/src/ui
	COPY	ui		.
	
	#cspell:words phpize
	RUN :	true										\
		&&	phpize										\
		&&	./configure									\
		&&	make clean									\
		&&	make										\
		&&	true										;
