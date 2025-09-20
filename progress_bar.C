void progress_bar(ULong64_t current_event, ULong64_t total_event) {
	static int last_percent = -1;
	static int last_total   = -1;

	const int bar_width = 80;
	const char* FILLED = "█";
	const char* EMPTY  = "░";

	if (total_event <= 0) return;

	if (total_event != last_total || current_event <= 1) {
		last_percent = -1;
		last_total   = total_event;
	}

	if (current_event < 0) current_event = 0;
	if (current_event > total_event) current_event = total_event;

	int percent = static_cast<int>(std::floor((double)current_event * 100.0 / total_event));
	if (percent > 100) percent = 100;

	if (percent != last_percent || current_event == total_event) {
		int filled = static_cast<int>(std::round(bar_width * (percent / 100.0)));

		std::cout << "[";
		for (int i = 0; i < bar_width; ++i) {
			if (i < filled) std::cout << FILLED;   // filled block - \u2588 or U+2588 FULL BLOCK
			else std::cout << EMPTY;           // empty block  - \u2591 or U+2591 LIGHT SHADE
		}
		std::cout << "] " << std::setw(3) << percent << "%\r";
		std::cout.flush();

		if (current_event == total_event) std::cout << std::endl; // newline for the last entry
	}
}
