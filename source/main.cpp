/*
 * Copyright © 2012 Fabian Schuiki
 */
#include "Application.h"


int main(int argc, char * argv[])
{
	Application *app = new Application;
	int rc = app->run(argc, argv);
	delete app;
	return rc;
}