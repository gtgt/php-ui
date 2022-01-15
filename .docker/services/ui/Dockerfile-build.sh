#!/usr/bin/env bash
	set -e
	
	ABSNAME0=$(cd "$(dirname "$0")"&&echo "$PWD")	#cspell:ignore absname
	
	names=({,php-}Dockerfile) # [1] is both raw path and argument to `sed` command `r`
	
	clean	(){
		((						  noExport))||
		rm -fr			"$ABSNAME0"/export
		rm -f			"$ABSNAME0"/"${names[1]}"
		}
	
	main	(){
		cd				"$ABSNAME0"
		
		clean
		
		((						  noExport))||{
			mkdir -p	"$ABSNAME0"/export
			cd			"$ABSNAME0"/export;		url=(	proto='https' '://' host='github.com' #cspell:words proto
																		'/' organization='docker-library'
																		'/' repo='php' '/branches/'	branch='master'
																		'/' basedir='7.4/bullseye/'	basename='cli')
			inpolate							url\
											 "${url[@]}"
			# cf https://stackoverflow.com/questions/7106012/download-a-single-folder-or-directory-from-a-github-repo#comment124999005_18194523
			svn						export	 "${url}"
			mkdir -p	"$ABSNAME0"/docker-php
#			. "$(vpath vbash base)"&&set +f&&_vnq\ 
			cp -a			$( ((!noClobber))||printf %s -n)									 "${basename}"/docker-php-*\
						"$ABSNAME0"/docker-php
			cp -a			$( ((!noClobber))||printf %s -n)									 "${basename}"/Dockerfile\
						"$ABSNAME0"/"${names[0]}"
			cd			"$ABSNAME0";}
		
		((!editPatch))||{
			cp -avn	"${names[@]}"		||:	;
			! read -sp "Edit any way you see fit, $(stty-a_get eof) to skip or \return to run: vi ${names[1]}"||
			vi		"${names[1]}"			;echo >&2
			diff -u	"${names[@]}"			\
				>	"${names[1]}".patch	||:	;}
		patch	<	"${names[1]}".patch
		mv			"${names[@]}"
		
		sed -Ei ''	-e	"/^[[:space:]]*(COPY)[[:space:]]+(docker-php)-/s!(docker-php)-!ui/.docker/services/ui/\1/\1-!g"\
					"${names[1]}"
		sed -Ei ''	-e	"/^[[:space:]]*(ENTRYPOINT|CMD)/d"\
					"${names[1]}" # mostly to rid warnings like "only the last one will be used"
		sed -E		-e "/^[[:space:]]*(FROM)[[:space:]]+(INCLUDE):([^[:space:]]*)[[:space:]]*\$/{
				r	 ${names[1]}
				d
				}"	Dockerfile-build\
				>	"${names[0]}"
		
		clean
		}
	
	# mnemonic: inline interpolation #cspell:words inpolate
	# usage: inpolate var [var=str || str]...
	inpolate(){ # (intentionally w/o declare -n)
		declare					inpolate_n=$1					inpolate_t && shift
		declare					inpolate_a						inpolate_r
		for						inpolate_a;do					inpolate_r+=$(
							)"${inpolate_a#*=}"
			[[				 "${inpolate_a//*=*}" ]]||{			inpolate_t=$(
							)"${inpolate_a#*=}"									\
				&&	eval	 "${inpolate_a%%=*}$(	)=$(	)\${inpolate_t}"	;};done\
		&&			eval	 "${inpolate_n}$(		)=$(	)\${inpolate_r}"		;}
	
	stty-a_get(){ stty -a|sed -Ene"/.*[[:space:]]$1 = ([^;]*).*/s//\\1/p";} #sync: ~vike/.bash_profile
	
	main
	
