	FROM gcc:latest as builder
	RUN : Pre-reqs										\
		&&	apt-get update								\
		&&	apt-get install -y --no-install-recommends	\
			meson ninja-build							\
			libgtk-3-dev								\
		&&	true										;
	
	WORKDIR			/usr/src/libui
	COPY	libui	.
	
	# Cheat sheet
	#	ARG name[=default]	# build-time only
	#	ENV name=set		#	run-time image persisted
	#	… ${name:-default}	#	any-time, any-where
	#	… ${name:+whenSet}	#	any-time, any-where
	
	# Failed attempt at colorizing meson/ninja in d-c build - f*ck it
	ARG		TERM=xterm-256color
	ENV		TERM=${TERM:-xterm-256color}
#	RUN	  : TERM=${TERM} # debug
	ENV		DEBUG_COLORS=true
	
	RUN : Build														\
		&&	meson setup --wipe	build								\
		&&	ninja -v -C			build								\
		&&	ninja -v -C			build install						\
		&&	ninja -v -C			build -t	compdb					\
			#cspell:ignore					compdb					#
				>				build/compile_commands.json			\
		&&	true													;

# Re "builder" stage: The following php image obviously contains enough of
#	buildkit to build libui (`apt install meson ninja-build` aside) but I'll
#	keep the stage separate to ease removal of dep on ui/../libui
# TODO: look into using separate Dockerfile
#	(multiple like `docker-compose -f 0 -f n`)

#	FROM php:7-cli
#
# NOTE: THIS DOCKERFILE IS GENERATED VIA "apply-templates.sh"
#
# PLEASE DO NOT EDIT IT DIRECTLY.
#

FROM debian:bullseye-slim

# prevent Debian's PHP packages from being installed
# https://github.com/docker-library/php/pull/542
RUN set -eux; \
	{ \
		echo 'Package: php*'; \
		echo 'Pin: release *'; \
		echo 'Pin-Priority: -1'; \
	} > /etc/apt/preferences.d/no-debian-php

# dependencies required for running "phpize"
# (see persistent deps below)
ENV PHPIZE_DEPS \
		autoconf \
		dpkg-dev \
		file \
		g++ \
		gcc \
		libc-dev \
		make \
		pkg-config \
		re2c

# persistent / runtime deps
RUN set -eux; \
	apt-get update; \
	apt-get install -y --no-install-recommends \
		$PHPIZE_DEPS \
		ca-certificates \
		curl \
		xz-utils \
	; \
	rm -rf /var/lib/apt/lists/*

ENV PHP_INI_DIR /usr/local/etc/php
RUN set -eux; \
	mkdir -p "$PHP_INI_DIR/conf.d"; \
# allow running as an arbitrary user (https://github.com/docker-library/php/issues/743)
	[ ! -d /var/www/html ]; \
	mkdir -p /var/www/html; \
	chown www-data:www-data /var/www/html; \
	chmod 777 /var/www/html

# Apply stack smash protection to functions using local buffers and alloca()
# Make PHP's main executable position-independent (improves ASLR security mechanism, and has no performance impact on x86_64)
# Generate debug information (-g) and disable optimization (-O0)
# Enable linker optimization (this sorts the hash buckets to improve cache locality, and is non-default)
# https://github.com/docker-library/php/issues/272
# -D_LARGEFILE_SOURCE and -D_FILE_OFFSET_BITS=64 (https://www.php.net/manual/en/intro.filesystem.php)
ENV PHP_CFLAGS="-fstack-protector-strong -fpic -fpie -g -O0 -D_LARGEFILE_SOURCE -D_FILE_OFFSET_BITS=64"
ENV PHP_CPPFLAGS="$PHP_CFLAGS"
ENV PHP_LDFLAGS="-Wl,-O1 -pie"

ENV GPG_KEYS 42670A7FE4D0441C8E4632349E4FDC074A4EF02D 5A52880781F755608BF815FC910DEB46F53EA312

ENV PHP_VERSION 7.4.27
ENV PHP_URL="https://www.php.net/distributions/php-7.4.27.tar.xz" PHP_ASC_URL="https://www.php.net/distributions/php-7.4.27.tar.xz.asc"
ENV PHP_SHA256="3f8b937310f155822752229c2c2feb8cc2621e25a728e7b94d0d74c128c43d0c"

RUN set -eux; \
	\
	savedAptMark="$(apt-mark showmanual)"; \
	apt-get update; \
	apt-get install -y --no-install-recommends gnupg dirmngr; \
	rm -rf /var/lib/apt/lists/*; \
	\
	mkdir -p /usr/src; \
	cd /usr/src; \
	\
	curl -fsSL -o php.tar.xz "$PHP_URL"; \
	\
	if [ -n "$PHP_SHA256" ]; then \
		echo "$PHP_SHA256 *php.tar.xz" | sha256sum -c -; \
	fi; \
	\
	if [ -n "$PHP_ASC_URL" ]; then \
		curl -fsSL -o php.tar.xz.asc "$PHP_ASC_URL"; \
		export GNUPGHOME="$(mktemp -d)"; \
		for key in $GPG_KEYS; do \
			gpg --batch --keyserver keyserver.ubuntu.com --recv-keys "$key"; \
		done; \
		gpg --batch --verify php.tar.xz.asc php.tar.xz; \
		gpgconf --kill all; \
		rm -rf "$GNUPGHOME"; \
	fi; \
	\
	apt-mark auto '.*' > /dev/null; \
	apt-mark manual $savedAptMark > /dev/null; \
	apt-get purge -y --auto-remove -o APT::AutoRemove::RecommendsImportant=false

COPY ui/.docker/services/ui/docker-php/docker-php-source /usr/local/bin/

RUN set -eux; \
	\
	savedAptMark="$(apt-mark showmanual)"; \
	apt-get update; \
	apt-get install -y --no-install-recommends \
		libargon2-dev \
		libcurl4-openssl-dev \
		libonig-dev \
		libreadline-dev \
		libsodium-dev \
		libsqlite3-dev \
		libssl-dev \
		libxml2-dev \
		zlib1g-dev \
	; \
	\
	export \
		CFLAGS="$PHP_CFLAGS" \
		CPPFLAGS="$PHP_CPPFLAGS" \
		LDFLAGS="$PHP_LDFLAGS" \
	; \
	docker-php-source extract; \
	cd /usr/src/php; \
	gnuArch="$(dpkg-architecture --query DEB_BUILD_GNU_TYPE)"; \
	debMultiarch="$(dpkg-architecture --query DEB_BUILD_MULTIARCH)"; \
# https://bugs.php.net/bug.php?id=74125
	if [ ! -d /usr/include/curl ]; then \
		ln -sT "/usr/include/$debMultiarch/curl" /usr/local/include/curl; \
	fi; \
	./configure \
		--build="$gnuArch" \
		--with-config-file-path="$PHP_INI_DIR" \
		--with-config-file-scan-dir="$PHP_INI_DIR/conf.d" \
		\
# make sure invalid --configure-flags are fatal errors instead of just warnings
		--enable-option-checking=fatal \
		\
# https://github.com/docker-library/php/issues/439
		--with-mhash \
		\
# https://github.com/docker-library/php/issues/822
		--with-pic \
		\
# --enable-ftp is included here because ftp_ssl_connect() needs ftp to be compiled statically (see https://github.com/docker-library/php/issues/236)
		--enable-ftp \
# --enable-mbstring is included here because otherwise there's no way to get pecl to use it properly (see https://github.com/docker-library/php/issues/195)
		--enable-mbstring \
# --enable-mysqlnd is included here because it's harder to compile after the fact than extensions are (since it's a plugin for several extensions, not an extension in itself)
		--enable-mysqlnd \
# https://wiki.php.net/rfc/argon2_password_hash
		--with-password-argon2 \
# https://wiki.php.net/rfc/libsodium
		--with-sodium=shared \
# always build against system sqlite3 (https://github.com/php/php-src/commit/6083a387a81dbbd66d6316a3a12a63f06d5f7109)
		--with-pdo-sqlite=/usr \
		--with-sqlite3=/usr \
		\
		--with-curl \
		--with-openssl \
		--with-readline \
		--with-zlib \
		\
# in PHP 7.4+, the pecl/pear installers are officially deprecated (requiring an explicit "--with-pear")
		--with-pear \
		\
# bundled pcre does not support JIT on s390x
# https://manpages.debian.org/bullseye/libpcre3-dev/pcrejit.3.en.html#AVAILABILITY_OF_JIT_SUPPORT
		$(test "$gnuArch" = 's390x-linux-gnu' && echo '--without-pcre-jit') \
		--with-libdir="lib/$debMultiarch" \
		\
# https://github.com/docker-library/php/pull/939#issuecomment-730501748
		--enable-embed \
	; \
	make -j "$(nproc)"; \
	find -type f -name '*.a' -delete; \
	make install; \
	find \
		/usr/local \
		-type f \
		-perm '/0111' \
	; \
	make clean; \
	\
# https://github.com/docker-library/php/issues/692 (copy default example "php.ini" files somewhere easily discoverable)
	cp -v php.ini-* "$PHP_INI_DIR/"; \
	\
	cd /; \
	\
# reset apt-mark's "manual" list so that "purge --auto-remove" will remove all build dependencies
	apt-mark auto '.*' > /dev/null; \
	[ -z "$savedAptMark" ] || apt-mark manual $savedAptMark; \
	find /usr/local -type f -executable -exec ldd '{}' ';' \
		| awk '/=>/ { print $(NF-1) }' \
		| sort -u \
		| xargs -r dpkg-query --search \
		| cut -d: -f1 \
		| sort -u \
		| xargs -r apt-mark manual \
	; \
	apt-get purge -y --auto-remove -o APT::AutoRemove::RecommendsImportant=false; \
	rm -rf /var/lib/apt/lists/*; \
	\
# update pecl channel definitions https://github.com/docker-library/php/issues/443
	pecl update-channels; \
	rm -rf /tmp/pear ~/.pearrc; \
	\
# smoke test
	php --version

COPY ui/.docker/services/ui/docker-php/docker-php-ext-* ui/.docker/services/ui/docker-php/docker-php-entrypoint /usr/local/bin/

# sodium was built as a shared module (so that it can be replaced later if so desired), so let's enable it too (https://github.com/docker-library/php/issues/598)
RUN docker-php-ext-enable sodium

	
	RUN docker-php-source extract
	
	RUN : Base reqs													\
		&& apt-get			update									\
		&& apt-get			install -y --no-install-recommends		\
			libgtk-3-dev											\
		&& true														;
	
	RUN : Extra reqs												\
		&& apt-get			update									\
		&& apt-get			install -y --no-install-recommends		\
			gawk													\
			bbe														\
			bc														\
			source-highlight										\
		&& apt-get			install -y --no-install-recommends		\
			npm														\
		&&	npm install -g diff-so-fancy							\
		&& true														;
	
	RUN : Optional "complex" tools									\
		#
		&&	apt-get			update									\
		&&	apt-get			install -y --no-install-recommends		\
									gdb								\
									clang							\
									clangd							\
			#cspell:ignore			clangd							#
			python3-pip												\
		&&			pip		install	compiledb						\
			#cspell:ignore			compiledb						#
		#
#		# For running .docker/services/ui/Dockerfile-build.sh		#
#		&&	apt-get			update									\
#		&&	apt-get			install -y --no-install-recommends		\
#			subversion												\
#		#
#		# For stepping in C debugger								# (`sudo pear ...` for system domain in non-root devcontainer; also note `pear config-...` as I still see it like https://pear.php.net/bugs/bug.php?id=13123 said to be fixed 2008 but next to only hit on https://www.google.com/search?q="config-set"+"failed,+channel+pecl.php.net")
#		&&			pear	config-set		temp_dir /usr/src/pecl	\
#		&&	(cd "$(	pear	config-get		temp_dir		)"	\
#			&&		pecl	bundle			xdebug				)	\
		#
		# Common							Xdebug (PHP)			#
		&&			pecl	install			xdebug					\
			#cspell:ignore pecl										#
#		&&	 docker-php-ext-enable			xdebug					\
		# I'll control it w/ args "-d", "zend_extension=xdebug" in .vscode/{tasks,launch}.json instead
		#	(manual: enable w/ `sudo -E docker-php-ext-enable xdebug`
		#		and	disable by `sudo rm /usr/local/etc/php/conf.d/docker-php-ext-xdebug.ini`)
		#
		# For rebuilding libui.			Alternative manual install:	 sudo apt update && sudo apt install meson ninja-build # meson is req'd also in some `meson --internal symbolextractor` run by `ninja -C build`
		&&	apt-get			update									\
		&&	apt-get			install -y --no-install-recommends		\
			meson ninja-build										\
		#															#
		&&	true													;
	
	RUN : Optional "simple" tools									\
		&&	apt-get			update									\
		&&	apt-get			install -y --no-install-recommends		\
			less													\
			vim														\
		#	vIm should prolly be considered complex for some but	#
		#		my uses are rather simple (I mainly use vsCode)		#
		&&	true													;
	
	COPY --from=builder	/usr/src/libui/build/meson-out/libui.so.0	/usr/local/lib/x86_64-linux-gnu/
	RUN ln -s										   libui.so.0	/usr/local/lib/x86_64-linux-gnu/libui.so
	COPY --from=builder	/usr/src/libui/ui_unix.h					/usr/local/include/
	COPY --from=builder	/usr/src/libui/ui.h							/usr/local/include/
	
	# For testing example like /usr/lib-src/libui/build/meson-out/histogram
	RUN mkdir -p													/usr/src
	COPY --from=builder	/usr/src/libui								/usr/src/libui
	
	#cspell:ignore devcontainer nonroot moby buildkit autoremove
	ENV DEVCONTAINER=ui/.devcontainer
	
	# I'm having trouble getting this picked up when running `docker-compose build`:
	# docker-compose.yml build: args: USERNAME: vscode
	ARG USERNAME=vscode
	# but NOTE that, even with above and next `ARG USERNAME=automatic` commented-out,
	# I still only get it working with:
	#
	# NB: `docker-compose build --build-arg USERNAME=vscode`
	
	# BEG vsCode remote container add existing - docker from compose
	
	# [Option] Install zsh
	ARG INSTALL_ZSH="true"
	# [Option] Upgrade OS packages to their latest versions
	ARG UPGRADE_PACKAGES="false"
	# [Option] Enable non-root Docker access in container
	ARG ENABLE_NONROOT_DOCKER="true"
	# [Option] Use the OSS Moby CLI instead of the licensed Docker CLI
	ARG USE_MOBY="true"

	# Enable new "BUILDKIT" mode for Docker CLI
	ENV DOCKER_BUILDKIT=1

	# Install needed packages and setup non-root user. Use a separate RUN statement to add your
	# own dependencies. A user of "automatic" attempts to reuse an user ID if one already exists.
#	ARG USERNAME=automatic
	ARG USER_UID=1000
	ARG USER_GID=$USER_UID
	COPY $DEVCONTAINER/library-scripts/*.sh /tmp/library-scripts/
	# edited prev & next(2)
	RUN : $DEVCONTAINER/library-scripts \
		&& apt-get update \
		&& /bin/bash /tmp/library-scripts/common-debian.sh "${INSTALL_ZSH}" "${USERNAME}" "${USER_UID}" "${USER_GID}" "${UPGRADE_PACKAGES}" "true" "true" \
		# Use Docker script from script library to set things up
		&& /bin/bash /tmp/library-scripts/docker-debian.sh "${ENABLE_NONROOT_DOCKER}" "/var/run/docker-host.sock" "/var/run/docker.sock" "${USERNAME}" \
		# Clean up
		&& apt-get autoremove -y && apt-get clean -y && rm -rf /var/lib/apt/lists/* /tmp/library-scripts/

	# Setting the ENTRYPOINT to docker-init.sh will configure non-root access 
	# to the Docker socket. The script will also execute CMD as needed.
	ENTRYPOINT [ "/usr/local/share/docker-init.sh" ]
	
	# END vsCode remote container add existing - docker from compose
	
	# next USERNAME: see above
	# BEG https://code.visualstudio.com/remote/advancedcontainers/avoid-extension-reinstalls
	
	RUN mkdir -p /home/$USERNAME/.vscode-server/extensions \
			/home/$USERNAME/.vscode-server-insiders/extensions \
		&& chown -R $USERNAME \
			/home/$USERNAME/.vscode-server \
			/home/$USERNAME/.vscode-server-insiders
	
	# END https://code.visualstudio.com/remote/advancedcontainers/avoid-extension-reinstalls
	
	# next SHOULD not be relevant at build-time
	ARG DISPLAY=host.docker.internal:0
	ENV DISPLAY=${DISPLAY:-host.docker.internal:0}
	
	WORKDIR				/usr/src/ui
	
	CMD [ "sleep", "infinity" ]
	
