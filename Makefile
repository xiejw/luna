${BUILD}:
	mkdir -p ${BUILD}

test: compile

fmt:
	~/Workspace/y/tools/clang_format_all.sh .

clean:
	rm -rf ${BUILD}
