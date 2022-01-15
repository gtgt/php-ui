#!/usr/bin/env bash
source "$(dirname "$BASH_SOURCE")/main/dev.sh"
C=(	"${C[0]}" $(let '			VSCODE_HELPER									'	\
		||	echo	-X\&	env	XDEBUG_TRIGGER=xdbg\;								)	\
	--				\
					-d zend_extension=xdebug										\
					-d		extension=./modules/ui.so								\
																					--	\
	"${C[@]:1}")