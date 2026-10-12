
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cctype>

using namespace std;

int hexVal(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    return 0;
}

vector<int> decode(string s) {
    vector<int> b;
    if (s == "-") return b;
    for (size_t i = 0; i + 1 < s.size(); i += 2)
        b.push_back(hexVal(s[i]) * 16 + hexVal(s[i + 1]));
    return b;
}

bool sig(const vector<int>& b, initializer_list<int> v) {
    if (b.size() < v.size()) return false;
    return equal(v.begin(), v.end(), b.begin());
}

int main() {
    int N;
    cin >> N;

    int lus = 0, mensonges = 0, refuses = 0;

    for (int i = 0; i < N; ++i) {
        string nom, octets;
        long long taille;
        cin >> nom >> taille >> octets;

        vector<int> b = decode(octets);
        string format = "REFUSE";

        if (taille >= 4) {
            if (taille >= 8 && sig(b, {0x89, 0x50, 0x4E, 0x47}))
                format = "PNG";
            else if (sig(b, {0xFF, 0xD8, 0xFF}))
                format = "JPEG";
            else if (sig(b, {0x42, 0x4D}))
                format = "BMP";
            else if (sig(b, {0x71, 0x6F, 0x69, 0x66}))
                format = "QOI";
            else if (sig(b, {0x47, 0x49, 0x46, 0x38}))
                format = "GIF";
            else if (sig(b, {0x00, 0x00}) &&
                     b.size() >= 4 &&
                     (b[2] == 0x01 || b[2] == 0x02) &&
                     b[3] == 0x00)
                format = "ICO";
            else if (taille >= 10 && sig(b, {0x23, 0x3F}))
                format = "HDR";
            else if (sig(b, {0x76, 0x2F, 0x31, 0x01}))
                format = "EXR";
            else if (b.size() >= 2 && b[0] == 0x50 &&
                     b[1] >= 0x31 && b[1] <= 0x36) {
                if (b[1] == 0x31 || b[1] == 0x34)
                    format = "PBM";
                else if (b[1] == 0x32 || b[1] == 0x35)
                    format = "PGM";
                else
                    format = "PPM";
            }
            else if (taille >= 18 && b.size() >= 3 &&
                     (b[2] == 0x00 || b[2] == 0x01 ||
                      b[2] == 0x02 || b[2] == 0x03 ||
                      b[2] == 0x09 || b[2] == 0x0A ||
                      b[2] == 0x0B))
                format = "TGA";
            else {
                size_t p = 0;
                if (b.size() >= 3 && b[0] == 0xEF &&
                    b[1] == 0xBB && b[2] == 0xBF)
                    p = 3;

                while (p < b.size() &&
                       (b[p] == 0x20 || b[p] == 0x09 ||
                        b[p] == 0x0A || b[p] == 0x0D))
                    ++p;

                if (p + 5 <= b.size() &&
                    equal(vector<int>{0x3C, 0x3F, 0x78, 0x6D, 0x6C}.begin(),
                           vector<int>{0x3C, 0x3F, 0x78, 0x6D, 0x6C}.end(),
                           b.begin() + p))
                    format = "SVG";
                else if (p + 4 <= b.size() &&
                         b[p] == 0x3C && b[p + 1] == 0x73 &&
                         b[p + 2] == 0x76 && b[p + 3] == 0x67)
                    format = "SVG";
            }
        }

        if (format == "REFUSE") {
            cout << nom << " REFUSE\n";
            ++refuses;
            continue;
        }

        string ext;
        size_t point = nom.find_last_of('.');
        if (point != string::npos) {
            ext = nom.substr(point + 1);
            transform(ext.begin(), ext.end(), ext.begin(),
                      [](unsigned char c) {
                          return static_cast<char>(tolower(c));
                      });
        }

        bool ok = false;
        if (format == "PNG") ok = ext == "png";
        else if (format == "JPEG") ok = ext == "jpg" || ext == "jpeg";
        else if (format == "BMP") ok = ext == "bmp";
        else if (format == "QOI") ok = ext == "qoi";
        else if (format == "GIF") ok = ext == "gif";
        else if (format == "ICO") ok = ext == "ico" || ext == "cur";
        else if (format == "HDR") ok = ext == "hdr";
        else if (format == "EXR") ok = ext == "exr";
        else if (format == "PBM") ok = ext == "pbm";
        else if (format == "PGM") ok = ext == "pgm";
        else if (format == "PPM") ok = ext == "ppm";
        else if (format == "TGA") ok = ext == "tga";
        else if (format == "SVG") ok = ext == "svg";

        cout << nom << ' ' << format << ' '
             << (ok ? "OK" : "MENT") << '\n';

        ++lus;
        if (!ok) ++mensonges;
    }

    cout << "LUS " << lus << '\n';
    cout << "MENSONGES " << mensonges << '\n';
    cout << "REFUSES " << refuses << '\n';

    return 0;
}