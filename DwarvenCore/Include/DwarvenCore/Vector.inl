#pragma once

template<typename T>
void df::AddUnique(Vector<T>& vector, const T& element) {
	for (const auto& el : vector) {
		if (el == element) {
			return;
		}
	}

	vector.push_back(element);
}

template<typename T>
auto df::FindElement(df::Vector<T>& vector, const T& element) -> int {
	const int count = int(vector.size());
	for (int i = 0; i < count; ++i) {
		if (vector[i] == element) {
			return i;
		}
	}

	return -1;
}

template<typename T>
auto df::RemoveOrdered(df::Vector<T>& vector, const T& element) -> int {
	const int count = int(vector.size());
	for (int i = 0; i < count; ++i) {
		if (vector[i] != element) {
			continue;
		}

		if (i != (count - 1)) {
			for (int j = i + 1; j < count; ++j) {
				vector[j - 1] = vector[j];
			}
		}

		vector.pop_back();

		return i;
	}

	return -1;
}

template<typename T>
auto df::RemoveUnordered(df::Vector<T>& vector, const T& element) -> int {
	const int count = int(vector.size());
	for (int i = 0; i < count; ++i) {
		if (vector[i] != element) {
			continue;
		}

		if (i != (count - 1)) {
			vector[i] = vector[count - 1];
		}

		vector.pop_back();

		return i;
	}

	return -1;
}