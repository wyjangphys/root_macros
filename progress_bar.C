#include <sys/ioctl.h>
#include <unistd.h>

int get_terminal_width();
void progress_bar(ULong64_t current_event, ULong64_t total_event);

int get_terminal_width() {
	if (!isatty(STDOUT_FILENO)) return -1;
	struct winsize w;
	if ( ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == -1 ) {
		return 80; // default window width
	}
	return w.ws_col;
}

void progress_bar(ULong64_t current_event, ULong64_t total_event) {
	static int last_percent = -1;
	static int last_total   = -1;
	static int last_cols    = -2;

	int cols = get_terminal_width();
	if( cols < 0 ) cols = 80;
	const char* FILLED = "█";
	const char* EMPTY_BLK  = "░";

	if (total_event <= 0) return;

	if (total_event != last_total || current_event <= 1) {
		last_percent = -1;
		last_total   = total_event;
		last_cols    = -2;
	}

	if (current_event < 0) current_event = 0;
	if (current_event > total_event) current_event = total_event;

	int percent = static_cast<int>(std::floor((double)current_event * 100.0 / total_event));
	if (percent > 100) percent = 100;

  int num_digits_total = std::to_string(total_event).length();
	int reserved = 10 + 2 * num_digits_total;
	int bar_width = cols - reserved;
	if (bar_width < 10) bar_width = 10;

	bool need_redraw = (percent != last_percent) || (cols != last_cols) || (current_event == total_event);
	if (!need_redraw) return;

	int filled = static_cast<int>(std::round(bar_width * (percent / 100.0)));
	if (filled < 0) filled = 0;
	if (filled> bar_width) filled = bar_width;

	std::cout << "\r[";
	for (int i = 0; i < bar_width; ++i) {
		if (i < filled) std::cout << FILLED;   // filled block - \u2588 or U+2588 FULL BLOCK
		else std::cout << EMPTY_BLK;           // empty block  - \u2591 or U+2591 LIGHT SHADE
	}
	std::cout << "] " << "(" << current_event << "/" << total_event << ")" << std::setw(3) << percent << "%";
	std::cout.flush();

	last_percent = percent;
	last_cols    = cols;

	if (current_event == total_event) std::cout << std::endl; // newline for the last entry
}
