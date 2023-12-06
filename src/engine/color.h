#define INVALID_COLOR_INDEX (0xffff)

#if 1
static v3 MAGICAVOXEL_DEFAULT_PALETTE[u8_MAX+1] = {
  V3( 0x01, 0x01, 0x01), // 0
  V3( 0xff, 0xff, 0xff), // 1
  V3( 0xff, 0xff, 0xcc), // 2
  V3( 0xff, 0xff, 0x99), // 3
  V3( 0xff, 0xff, 0x66), // 4
  V3( 0xff, 0xff, 0x33), // 5
  V3( 0xff, 0xff, 0x01), // 6
  V3( 0xff, 0xcc, 0xff), // 7
  V3( 0xff, 0xcc, 0xcc), // 8
  V3( 0xff, 0xcc, 0x99), // 9
  V3( 0xff, 0xcc, 0x66), // 10
  V3( 0xff, 0xcc, 0x33), // 11
  V3( 0xff, 0xcc, 0x01), // 12
  V3( 0xff, 0x99, 0xff), // 13
  V3( 0xff, 0x99, 0xcc), // 14
  V3( 0xff, 0x99, 0x99), // 15
  V3( 0xff, 0x99, 0x66), // 16
  V3( 0xff, 0x99, 0x33), // 17
  V3( 0xff, 0x99, 0x01), // 18
  V3( 0xff, 0x66, 0xff), // 19
  V3( 0xff, 0x66, 0xcc), // 20
  V3( 0xff, 0x66, 0x99), // 21
  V3( 0xff, 0x66, 0x66), // 22
  V3( 0xff, 0x66, 0x33), // 23
  V3( 0xff, 0x66, 0x01), // 24
  V3( 0xff, 0x33, 0xff), // 25
  V3( 0xff, 0x33, 0xcc), // 26
  V3( 0xff, 0x33, 0x99), // 27
  V3( 0xff, 0x33, 0x66), // 28
  V3( 0xff, 0x33, 0x33), // 29
  V3( 0xff, 0x33, 0x01), // 30
  V3( 0xff, 0x01, 0xff), // 31
  V3( 0xff, 0x01, 0xcc), // 32
  V3( 0xff, 0x01, 0x99), // 33
  V3( 0xff, 0x01, 0x66), // 34
  V3( 0xff, 0x01, 0x33), // 35
  V3( 0xff, 0x01, 0x01), // 36
  V3( 0xcc, 0xff, 0xff), // 37
  V3( 0xcc, 0xff, 0xcc), // 38
  V3( 0xcc, 0xff, 0x99), // 39
  V3( 0xcc, 0xff, 0x66), // 40
  V3( 0xcc, 0xff, 0x33), // 41
  V3( 0xcc, 0xff, 0x01), // 42
  V3( 0xcc, 0xcc, 0xff), // 43
  V3( 0xcc, 0xcc, 0xcc), // 44
  V3( 0xcc, 0xcc, 0x99), // 45
  V3( 0xcc, 0xcc, 0x66), // 46
  V3( 0xcc, 0xcc, 0x33), // 47
  V3( 0xcc, 0xcc, 0x01), // 48
  V3( 0xcc, 0x99, 0xff), // 49
  V3( 0xcc, 0x99, 0xcc), // 50
  V3( 0xcc, 0x99, 0x99), // 51
  V3( 0xcc, 0x99, 0x66), // 52
  V3( 0xcc, 0x99, 0x33), // 53
  V3( 0xcc, 0x99, 0x01), // 54
  V3( 0xcc, 0x66, 0xff), // 55
  V3( 0xcc, 0x66, 0xcc), // 56
  V3( 0xcc, 0x66, 0x99), // 57
  V3( 0xcc, 0x66, 0x66), // 58
  V3( 0xcc, 0x66, 0x33), // 59
  V3( 0xcc, 0x66, 0x01), // 60
  V3( 0xcc, 0x33, 0xff), // 61
  V3( 0xcc, 0x33, 0xcc), // 62
  V3( 0xcc, 0x33, 0x99), // 63
  V3( 0xcc, 0x33, 0x66), // 64
  V3( 0xcc, 0x33, 0x33), // 65
  V3( 0xcc, 0x33, 0x01), // 66
  V3( 0xcc, 0x01, 0xff), // 67
  V3( 0xcc, 0x01, 0xcc), // 68
  V3( 0xcc, 0x01, 0x99), // 69
  V3( 0xcc, 0x01, 0x66), // 70
  V3( 0xcc, 0x01, 0x33), // 71
  V3( 0xcc, 0x01, 0x01), // 72
  V3( 0x99, 0xff, 0xff), // 73
  V3( 0x99, 0xff, 0xcc), // 74
  V3( 0x99, 0xff, 0x99), // 75
  V3( 0x99, 0xff, 0x66), // 76
  V3( 0x99, 0xff, 0x33), // 77
  V3( 0x99, 0xff, 0x01), // 78
  V3( 0x99, 0xcc, 0xff), // 79
  V3( 0x99, 0xcc, 0xcc), // 80
  V3( 0x99, 0xcc, 0x99), // 81
  V3( 0x99, 0xcc, 0x66), // 82
  V3( 0x99, 0xcc, 0x33), // 83
  V3( 0x99, 0xcc, 0x01), // 84
  V3( 0x99, 0x99, 0xff), // 85
  V3( 0x99, 0x99, 0xcc), // 86
  V3( 0x99, 0x99, 0x99), // 87
  V3( 0x99, 0x99, 0x66), // 88
  V3( 0x99, 0x99, 0x33), // 89
  V3( 0x99, 0x99, 0x01), // 90
  V3( 0x99, 0x66, 0xff), // 91
  V3( 0x99, 0x66, 0xcc), // 92
  V3( 0x99, 0x66, 0x99), // 93
  V3( 0x99, 0x66, 0x66), // 94
  V3( 0x99, 0x66, 0x33), // 95
  V3( 0x99, 0x66, 0x01), // 96
  V3( 0x99, 0x33, 0xff), // 97
  V3( 0x99, 0x33, 0xcc), // 98
  V3( 0x99, 0x33, 0x99), // 99
  V3( 0x99, 0x33, 0x66), // 100
  V3( 0x99, 0x33, 0x33), // 101
  V3( 0x99, 0x33, 0x01), // 102
  V3( 0x99, 0x01, 0xff), // 103
  V3( 0x99, 0x01, 0xcc), // 104
  V3( 0x99, 0x01, 0x99), // 105
  V3( 0x99, 0x01, 0x66), // 106
  V3( 0x99, 0x01, 0x33), // 107
  V3( 0x99, 0x01, 0x01), // 108
  V3( 0x66, 0xff, 0xff), // 109
  V3( 0x66, 0xff, 0xcc), // 110
  V3( 0x66, 0xff, 0x99), // 111
  V3( 0x66, 0xff, 0x66), // 112
  V3( 0x66, 0xff, 0x33), // 113
  V3( 0x66, 0xff, 0x01), // 114
  V3( 0x66, 0xcc, 0xff), // 115
  V3( 0x66, 0xcc, 0xcc), // 116
  V3( 0x66, 0xcc, 0x99), // 117
  V3( 0x66, 0xcc, 0x66), // 118
  V3( 0x66, 0xcc, 0x33), // 119
  V3( 0x66, 0xcc, 0x01), // 120
  V3( 0x66, 0x99, 0xff), // 121
  V3( 0x66, 0x99, 0xcc), // 122
  V3( 0x66, 0x99, 0x99), // 123
  V3( 0x66, 0x99, 0x66), // 124
  V3( 0x66, 0x99, 0x33), // 125
  V3( 0x66, 0x99, 0x01), // 126
  V3( 0x66, 0x66, 0xff), // 127
  V3( 0x66, 0x66, 0xcc), // 128
  V3( 0x66, 0x66, 0x99), // 129
  V3( 0x66, 0x66, 0x66), // 130
  V3( 0x66, 0x66, 0x33), // 131
  V3( 0x66, 0x66, 0x01), // 132
  V3( 0x66, 0x33, 0xff), // 133
  V3( 0x66, 0x33, 0xcc), // 134
  V3( 0x66, 0x33, 0x99), // 135
  V3( 0x66, 0x33, 0x66), // 136
  V3( 0x66, 0x33, 0x33), // 137
  V3( 0x66, 0x33, 0x01), // 138
  V3( 0x66, 0x01, 0xff), // 139
  V3( 0x66, 0x01, 0xcc), // 140
  V3( 0x66, 0x01, 0x99), // 141
  V3( 0x66, 0x01, 0x66), // 142
  V3( 0x66, 0x01, 0x33), // 143
  V3( 0x66, 0x01, 0x01), // 144
  V3( 0x33, 0xff, 0xff), // 145
  V3( 0x33, 0xff, 0xcc), // 146
  V3( 0x33, 0xff, 0x99), // 147
  V3( 0x33, 0xff, 0x66), // 148
  V3( 0x33, 0xff, 0x33), // 149
  V3( 0x33, 0xff, 0x01), // 150
  V3( 0x33, 0xcc, 0xff), // 151
  V3( 0x33, 0xcc, 0xcc), // 152
  V3( 0x33, 0xcc, 0x99), // 153
  V3( 0x33, 0xcc, 0x66), // 154
  V3( 0x33, 0xcc, 0x33), // 155
  V3( 0x33, 0xcc, 0x01), // 156
  V3( 0x33, 0x99, 0xff), // 157
  V3( 0x33, 0x99, 0xcc), // 158
  V3( 0x33, 0x99, 0x99), // 159
  V3( 0x33, 0x99, 0x66), // 160
  V3( 0x33, 0x99, 0x33), // 161
  V3( 0x33, 0x99, 0x01), // 162
  V3( 0x33, 0x66, 0xff), // 163
  V3( 0x33, 0x66, 0xcc), // 164
  V3( 0x33, 0x66, 0x99), // 165
  V3( 0x33, 0x66, 0x66), // 166
  V3( 0x33, 0x66, 0x33), // 167
  V3( 0x33, 0x66, 0x01), // 168
  V3( 0x33, 0x33, 0xff), // 169
  V3( 0x33, 0x33, 0xcc), // 170
  V3( 0x33, 0x33, 0x99), // 171
  V3( 0x33, 0x33, 0x66), // 172
  V3( 0x33, 0x33, 0x33), // 173
  V3( 0x33, 0x33, 0x01), // 174
  V3( 0x33, 0x01, 0xff), // 175
  V3( 0x33, 0x01, 0xcc), // 176
  V3( 0x33, 0x01, 0x99), // 177
  V3( 0x33, 0x01, 0x66), // 178
  V3( 0x33, 0x01, 0x33), // 179
  V3( 0x33, 0x01, 0x01), // 180
  V3( 0x01, 0xff, 0xff), // 181
  V3( 0x01, 0xff, 0xcc), // 182
  V3( 0x01, 0xff, 0x99), // 183
  V3( 0x01, 0xff, 0x66), // 184
  V3( 0x01, 0xff, 0x33), // 185
  V3( 0x01, 0xff, 0x01), // 186
  V3( 0x01, 0xcc, 0xff), // 187
  V3( 0x01, 0xcc, 0xcc), // 188
  V3( 0x01, 0xcc, 0x99), // 189
  V3( 0x01, 0xcc, 0x66), // 190
  V3( 0x01, 0xcc, 0x33), // 191
  V3( 0x01, 0xcc, 0x01), // 192
  V3( 0x01, 0x99, 0xff), // 193
  V3( 0x01, 0x99, 0xcc), // 194
  V3( 0x01, 0x99, 0x99), // 195
  V3( 0x01, 0x99, 0x66), // 196
  V3( 0x01, 0x99, 0x33), // 197
  V3( 0x01, 0x99, 0x01), // 198
  V3( 0x01, 0x66, 0xff), // 199
  V3( 0x01, 0x66, 0xcc), // 200
  V3( 0x01, 0x66, 0x99), // 201
  V3( 0x01, 0x66, 0x66), // 202
  V3( 0x01, 0x66, 0x33), // 203
  V3( 0x01, 0x66, 0x01), // 204
  V3( 0x01, 0x33, 0xff), // 205
  V3( 0x01, 0x33, 0xcc), // 206
  V3( 0x01, 0x33, 0x99), // 207
  V3( 0x01, 0x33, 0x66), // 208
  V3( 0x01, 0x33, 0x33), // 209
  V3( 0x01, 0x33, 0x01), // 210
  V3( 0x01, 0x01, 0xff), // 211
  V3( 0x01, 0x01, 0xcc), // 212
  V3( 0x01, 0x01, 0x99), // 213
  V3( 0x01, 0x01, 0x66), // 214
  V3( 0x01, 0x01, 0x33), // 215
  V3( 0xee, 0x01, 0x01), // 216
  V3( 0xdd, 0x01, 0x01), // 217
  V3( 0xbb, 0x01, 0x01), // 218
  V3( 0xaa, 0x01, 0x01), // 219
  V3( 0x88, 0x01, 0x01), // 220
  V3( 0x77, 0x01, 0x01), // 221
  V3( 0x55, 0x01, 0x01), // 222
  V3( 0x44, 0x01, 0x01), // 223
  V3( 0x22, 0x01, 0x01), // 224
  V3( 0x11, 0x01, 0x01), // 225
  V3( 0x01, 0xee, 0x01), // 226
  V3( 0x01, 0xdd, 0x01), // 227
  V3( 0x01, 0xbb, 0x01), // 228
  V3( 0x01, 0xaa, 0x01), // 229
  V3( 0x01, 0x88, 0x01), // 230
  V3( 0x01, 0x77, 0x01), // 231
  V3( 0x01, 0x55, 0x01), // 232
  V3( 0x01, 0x44, 0x01), // 233
  V3( 0x01, 0x22, 0x01), // 234
  V3( 0x01, 0x11, 0x01), // 235
  V3( 0x01, 0x01, 0xee), // 236
  V3( 0x01, 0x01, 0xdd), // 237
  V3( 0x01, 0x01, 0xbb), // 238
  V3( 0x01, 0x01, 0xaa), // 239
  V3( 0x01, 0x01, 0x88), // 240
  V3( 0x01, 0x01, 0x77), // 241
  V3( 0x01, 0x01, 0x55), // 242
  V3( 0x01, 0x01, 0x44), // 243
  V3( 0x01, 0x01, 0x22), // 244
  V3( 0x01, 0x01, 0x11), // 245
  V3( 0xee, 0xee, 0xee), // 246
  V3( 0xdd, 0xdd, 0xdd), // 247
  V3( 0xbb, 0xbb, 0xbb), // 248
  V3( 0xaa, 0xaa, 0xaa), // 249
  V3( 0x88, 0x88, 0x88), // 250
  V3( 0x77, 0x77, 0x77), // 251
  V3( 0x55, 0x55, 0x55), // 252
  V3( 0x44, 0x44, 0x44), // 253
  V3( 0x22, 0x22, 0x22), // 254
  V3( 0x11, 0x11, 0x11), // 255
};
#endif

link_internal v3_cursor * GetColorPalette();
link_internal v3          GetColorData(u32 ColorIndex);