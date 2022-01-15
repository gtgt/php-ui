#!/usr/bin/env bash
	trap 'e=$?; set +x; ((!e))||echo >&2 "$0 error ($e${BASH_LINENO:+ @ line $BASH_LINENO})"; trap - exit; builtin exit $e' exit
	set -e
	shopt -s extglob
	:;	declare -n nameref || ! echo >&2 "Unmet requirement in $BASH (known working version 5.1 versus your $BASH_VERSION):"
		declare -n nameref
	
	#	optionally existing utility
	:;	o()
		{				declare		u=(	"$1")&&shift	;
		:;!	&>/dev/null	type	 "${u}"					\
		||						 "${u}"	"$@"			;}
	
	#	print unless optionally existing utility
	:;	p()
		{				declare		u=(	"$1")&&shift	;
		:;	&>/dev/null	type	 "${u}"					\
		&&						 "${u}"	"$@"			\
		||						echo	"$1"			;}
	
	#	rest unless optionally existing utility
	:;	r()
		{				declare		u=(	"$1")&&shift	;
		:;	&>/dev/null	type	 "${u}"					\
		&&						 "${u}"	"$@"			\
		||								"$@"			;}
	

	
	# optional personal framework
	! vp=$(o vpath vbash base) || ! [[ -a "$vp" ]] || . "$vp"
	

	
	cwd=$0
	cwd="$(dirname	"$cwd")"
	cwd="$(cd		"$cwd" && echo "$PWD")" # cwd="$(p vpa "$cwd")"
	cwd="$(dirname	"$cwd")"
	cwd="$(p vpdr	"$cwd")"
	echo >&2		"$cwd"
	cd				"$cwd"
	

	
	project_name=ui
	environ=(
		-			TERM
			=	 "${TERM}"
		-			COLUMNS
			=	 "${COLUMNS:-$(	tput cols)}"
		-			LINES
			=	 "${LINES:-$(	tput lines)}"
		-			DISPLAY
			=		host.docker.internal:0
		-			PHP_LIBRARIES_ROOT
			=	 "${PHP_LIBRARIES_ROOT}"
		-		   VPHP_LIBRARIES_ROOTS
			=	"${VPHP_LIBRARIES_ROOTS}"
		)
	
	spec2args(){ declare -n spec=$1 args=$1_args && args=()
		:;		((								k=-1	))				;															 ((	 c=0))||:
		while	((				 ${#spec[@]}>++	k		));do
			case				 "${spec[		k		]}" in(!(*))
				;;(!(-))													! echo >&2 "missing '$(echo -	)' in $1+=(- $3 $4 $5) #$((	c	))":\
					\(			 "${spec[@]:	k: 3	 }" … \)
				;;(@(-))[[		 "${spec[		k+ 2	]}"	!=	 $4	]]	&&
					args+=($2	 "${spec[		k+=1	]}"$(		))	||
					args+=($2	 "${spec[		k+=1	]}"$(		\
								)"${spec[		k+=1	]}"$(		\
								)"${spec[		k+=1	]}"$(		))	;															 (( ++c	))
	#				o d "\${args[*][-2]}:${args[*]:${#args[*]}-2:2}"
				esac;done;}
	# This can of course be written without `declare -n` as specific loop(s) w/ positional params hardcoded
	spec2args environ -e name = value
	

	
#	o d environ_args
#	o d PWD
#	exit 157
	

	
	((!rebuild))||
	(	REMOVE_CNTRNR_suffixed_gcre="$project_name" && docker container	ls -a	|gawk -vs="$REMOVE_CNTRNR_suffixed_gcre" 'match($2,"_"s"$"){print $1}'|
			xargs docker container	rm && # gcre is ‘gawk compatible regex’
		REMOVE_IMAGES_suffixed_gcre="$project_name" && docker image		ls		|gawk -vs="$REMOVE_IMAGES_suffixed_gcre" 'match($1,"_"s"$"){print $3}'|
			xargs docker image		rm && # neither `d-c down --rmi all` nor `d-c build --no-cache` worked for me - i hate docker
		r _vnq				docker-compose build --build-arg TERM=$TERM)
	
	:;						docker-compose				()
	{ declare	 f
	f+=(		-f	.docker/docker-compose$(		).yml)
	[[ ! -e			.docker/docker-compose.local$(	).yml ]]||
	f+=(		-f	.docker/docker-compose.local$(	).yml)
	:;command				docker compose				\
						"${f[@]}"						\
						  "${@:1:1	}"					\
			 "${environ_args[@]}"						\
						  "${@:2}"						;} # god damn docker-compose didn't print anything and just did an exit 1 when it couldn't find a yml
	
	r _vnq					docker-compose up	"$project_name" -d # --no-build # dunno if rel: https://github.com/docker/compose/issues/8785
	
	((!${run_build-1}))||! >&2 echo "$0: run_build:"||
	r _vnq					docker-compose exec	"$project_name" bash .docker/compose/run_build.sh||:
	r _vnq					docker-compose exec	"$project_name" bash
		# ulimit -c unlimited && php -d extension=./modules/ui.so ./launch-task.php # gdb php -c core