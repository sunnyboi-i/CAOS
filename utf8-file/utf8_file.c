#include "utf8_file.h"

uint8_t count_byte(uint32_t byte) {
	if ((byte & 0b11100000) == 0b11000000) {
		return 2;
	} else if ((byte & 0b11110000) == 0b11100000) {
		return 3;
	} else if ((byte & 0b11111000) == 0b11110000) {
		return 4;
	} else if ((byte & 0b11111100) == 0b11111000) {
		return 5;
	} else if ((byte & 0b11111110) == 0b11111100) {
		return 6;
	} else if ((byte & 0b11111111) == 0b11111110) {
		return 7;
	}
	return 1;
}


uint8_t classify(uint32_t cp) {
	if (cp <= 127) {
		return 1;
	} else if (cp <= 2047) {
		return 2;
	} else if (cp <= 65535) {
		return 3;
	} else if (cp <= 1114111) {
		return 4;
	} else if (cp <= 67108863){
		return 5;
	} else if(cp <= 2147483647) {
		return 6;
	}
	return 7;
}
int utf8_write(utf8_file_t* f, const uint32_t* str, size_t count) {
	ssize_t writing;
	size_t quant = 0;
	while (quant < count) {
		uint32_t code_point = str[quant];
		uint8_t size = classify(code_point);
		uint8_t buffer[7];
		uint8_t bin = ~((1<<(8 - size)) - 1);

		uint32_t bin_a = 0b00111111;
		if (size > 6) {
			errno = EINVAL;
			printf("utf8_read failed: %s\n", strerror(errno));
			return -1;
		}
		if (size > 1) {
			buffer[0] = bin | (code_point >> (6 * (size - 1)));
			for (uint32_t i = 1; i < size; ++i) {
				buffer[i] = 0b10000000 | (((code_point >> (size - i - 1))) & bin_a);
			}
			writing = write(f->fd, buffer, size);
		} else {
			writing = write(f->fd, &code_point, sizeof(uint8_t));
		}
		if (writing < 1) {
			printf("%s\n", strerror(errno));
			return -1;
		} else if (writing == 0) {
			return quant;
		}
		quant += 1;
	}
	return quant;
}

int utf8_read(utf8_file_t* f, uint32_t* res, size_t count) {
  	ssize_t reading = 0;
		uint32_t quant = 0;
		while (quant < count) {
			uint8_t first_byte;
			reading = read(f->fd, &first_byte, sizeof(uint8_t));
			if (reading < 0) {
				printf("%s\n", strerror(errno));
				return -1;
			} else if (reading == 0) {
				return quant;
			}
			uint8_t size = count_byte(first_byte);
			if (size > 6) {
				errno = EINVAL;
				printf("utf8_write failed: %s\n", strerror(errno));
				return -1;
			}
			uint32_t code_p = first_byte & ((1 << (8 - size)) - 1);
			for (uint8_t i = 1; i < size; ++i) {
				uint8_t next_b;
				reading = read(f->fd, &next_b, sizeof(uint8_t));
				if (reading < 0) {
					printf("%s\n", strerror(errno));
					return -1;
				} else if (reading == 0) {
					return quant;
				}
				code_p = (code_p << 6) | (0b00111111 & next_b);
			}
			res[quant] = code_p;
			quant += 1;
		}

		return quant;

}

utf8_file_t* utf8_fromfd(int fd) {
    utf8_file_t* file = (utf8_file_t*)malloc(sizeof(utf8_file_t));
    file->fd = fd;
    return file;
}
