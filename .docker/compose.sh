#!/usr/bin/env bash
	trap '((!(number=$?)))||echo >&2 "$0 error ($number)"; trap - exit; builtin exit $number' exit
	set -e
	shopt -s extglob
	:;	declare -n nameref || ! echo >&2 "Unmet requirement in $BASH (known working version 5.1 versus your $BASH_VERSION):"
		declare -n nameref
	
	#	optionally existing function
	:;	o()
		{				declare		c=(	"$1")&&shift	;
		:;!	&>/dev/null	type	 "${c}"					\
		||						 "${c}"	"$@"			;}
	
	#	print unless optionally existing file command
	:;	p()
		{				declare		c=(	"$1")&&shift	;
		:;	&>/dev/null	type	 "${c}"					\
		&&						 "${c}"	"$@"			\
		||						echo	"$1"			;}
	

	
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
	volumes=(
		-		"$PHP_LIBRARIES_ROOT"
			:	"$PHP_LIBRARIES_ROOT"
		-		/opt/local/etc/php74$(	)/php.ini
			:	/usr/local/etc/php$(	)/php.ini
		-		"$cwd":/usr/src/"$project_name")
	environ=(
		-		VPHP_LIBRARIES_ROOTS
			= "$VPHP_LIBRARIES_ROOTS"
		-		PHP_LIBRARIES_ROOT
			= "$PHP_LIBRARIES_ROOT"
		-		COLUMNS
			= "$COLUMNS"
		-		LINES
			= "$LINES"
		-		TERM
			= "$TERM"
		-		DISPLAY
			=	host.docker.internal:0)
	
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
	# This can of course be written without `declare -n` as two specific loops w/ positional params hardcoded
	spec2args volumes -v src : dst
	spec2args environ -e name = value
	

	
#	o d volumes_args
#	o d environ_args
#	o d PWD
#	exit 157
	

	
	:;	docker-compose_run()
	{	docker-compose run			\
			"${volumes_args[@]}"	\
			"${environ_args[@]}"	\
						  "$@"		;}
	
	((!rebuild))||
	(REMOVE_IMAGES_suffixed_GCRE="$project_name" && docker image ls|gawk -vs="$REMOVE_IMAGES_suffixed_GCRE" 'match($1,"_"s"$"){print $3}'| # gcre is ‘gawk compatible regex’
		xargs docker image rm &&
		build --build-arg TERM=$TERM) # neither `d-c down --rmi all` nor `d-c build --no-cache` worked for me - i hate docker
	
	((!${run_build-1}))||! >&2 echo "$0: run_build:"||
	docker-compose_run				"$project_name" bash .docker/compose/run_build.sh||:
	docker-compose_run		--rm	"$project_name" bash
		# ulimit -c unlimited && php -d extension=./modules/ui.so ./launch-task.php # gdb php -c core