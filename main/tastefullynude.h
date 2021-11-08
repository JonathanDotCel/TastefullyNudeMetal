#pragma once

typedef void (*AppCoreFunction)(void);

void RunOnAppCore( AppCoreFunction func );