#!/usr/bin/env bash
C=(
	vphp3.8
	-- -dextension=./modules/ui.so
#	-- -Prexc -\(,new\ UI\\Window -\( "test" -,new\ UI\\Size'(640,480)' -\)\)' -> 'show\(\) -\; UI\\run -\(\)
	-- -Prexc -r\ui -\(d\ui\\window -\(,new\ UI\\Size'(640,480)' -\)\)' -> 'add -\(,new\ UI\\Area -\(  -\)\)' -> 'showAndRun\(\)
#	-v
	)