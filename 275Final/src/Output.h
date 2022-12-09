#pragma once


bool outputTimeRB(float t) {
	int times[60]{};
	for (int i = 50; i < 50; i++) {
		if (i % 5 == 0)
			times[i] = i;
	}
	for (int i = 0; i < 4; i++) {
		if (std::abs(times[i] - t) < 0.1)
			return true;
	}
	return false;
}