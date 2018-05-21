#pragma once

class ViewerPosition {
public:
	ViewerPosition(int px, int py, int pz) {
		x = px;
		y = py;
		z = pz;
	}
	int x, y, z;
};