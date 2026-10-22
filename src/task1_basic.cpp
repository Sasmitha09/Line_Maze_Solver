// Task 1 - Basic Coding
// Build & run:  g++ -std=c++11 -Wall -Wextra -o task1 src/task1_basic.cpp && ./task1
// Plain C++ (no Arduino calls), so the two functions can be reused in the ESP32 sketch (Task 9).

#include <cstdio>
#include <cmath>

// 1. Line-position estimation (weighted centroid)
//    position = sum(w_i * s_i) / sum(s_i),  w = {-2, -1, 0, +1, +2}
//    Returns false and leaves `position` unchanged when all readings are zero
//    (line lost), so the caller can keep its last valid position.
bool estimateLinePosition(const int s[5], float &position) {
    static const int weights[5] = {-2, -1, 0, 1, 2};

    long weightedSum = 0;
    long total = 0;
    for (int i = 0; i < 5; i++) {
        int v = (s[i] < 0) ? 0 : s[i];
        weightedSum += (long)weights[i] * v;
        total += v;
    }

    if (total == 0) return false;  // never divide by zero

    position = (float)weightedSum / (float)total;
    return true;
}

// 2. Motor-command limiting: clamp to [-maxMag, +maxMag], preserving sign.
int clampCommand(int cmd, int maxMag) {
    if (maxMag < 0) maxMag = -maxMag;
    if (cmd >  maxMag) return  maxMag;
    if (cmd < -maxMag) return -maxMag;
    return cmd;
}

void clampMotorCommands(int &left, int &right, int maxMag) {
    left  = clampCommand(left,  maxMag);
    right = clampCommand(right, maxMag);
}

// ---------------------------------------------------------------------------
// Examples and tests (desktop only)
// ---------------------------------------------------------------------------
static int failures = 0;

#define CHECK(cond)                                                        \
    do {                                                                   \
        if (!(cond)) {                                                     \
            printf("  FAIL (line %d): %s\n", __LINE__, #cond);             \
            failures++;                                                    \
        }                                                                  \
    } while (0)

static bool approxEqual(float a, float b) { return std::fabs(a - b) < 1e-4f; }

static void showPosition(const char *label, const int s[5]) {
    float pos = 99.0f;
    bool ok = estimateLinePosition(s, pos);
    printf("%-18s [%4d %4d %4d %4d %4d] -> ", label, s[0], s[1], s[2], s[3], s[4]);
    if (ok) printf("position = %+.3f\n", pos);
    else    printf("LINE LOST (no division performed)\n");
}

static void showClamp(int l, int r, int m) {
    int a = l, b = r;
    clampMotorCommands(a, b, m);
    printf("clamp(left=%4d, right=%4d, max=%3d) -> left=%4d, right=%4d\n", l, r, m, a, b);
}

int main() {
    printf("== Line-position estimation ==\n");
    int centred[5]  = {0, 0, 4000, 0, 0};
    int right[5]    = {0, 0, 1000, 3000, 0};
    int left[5]     = {2000, 2000, 0, 0, 0};
    int wide[5]     = {500, 1000, 2000, 1000, 500};
    int farRight[5] = {0, 0, 0, 0, 500};
    int farLeft[5]  = {500, 0, 0, 0, 0};
    int lost[5]     = {0, 0, 0, 0, 0};
    int badInput[5] = {-50, 0, 100, 0, 0};  // negative reading treated as 0

    showPosition("centred",        centred);   //  0
    showPosition("drifting right", right);     // +0.75
    showPosition("drifting left",  left);      // -1.5
    showPosition("symmetric wide", wide);      //  0
    showPosition("far right only", farRight);  // +2
    showPosition("far left only",  farLeft);   // -2
    showPosition("all zero",       lost);      // lost
    showPosition("negative input", badInput);  //  0

    float p = 99.0f;
    CHECK(estimateLinePosition(centred, p)  && approxEqual(p,  0.0f));
    CHECK(estimateLinePosition(right, p)    && approxEqual(p,  0.75f));
    CHECK(estimateLinePosition(left, p)     && approxEqual(p, -1.5f));
    CHECK(estimateLinePosition(wide, p)     && approxEqual(p,  0.0f));
    CHECK(estimateLinePosition(farRight, p) && approxEqual(p,  2.0f));
    CHECK(estimateLinePosition(farLeft, p)  && approxEqual(p, -2.0f));
    CHECK(estimateLinePosition(badInput, p) && approxEqual(p,  0.0f));
    p = 99.0f;
    CHECK(!estimateLinePosition(lost, p));
    CHECK(approxEqual(p, 99.0f));  // untouched when the line is lost

    // Caller side: remember the last valid position, steer toward it when lost.
    printf("\n== Lost-line handling (caller side) ==\n");
    int frames[4][5] = {
        {0, 0, 2000, 2000, 0},
        {0, 0, 0, 1500, 2500},
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
    };
    float lastPos = 0.0f;
    for (int f = 0; f < 4; f++) {
        float pos;
        if (estimateLinePosition(frames[f], pos)) {
            lastPos = pos;
            printf("frame %d: position = %+.3f\n", f, pos);
        } else {
            printf("frame %d: lost -> turn %s (last position %+.3f)\n",
                   f, lastPos >= 0 ? "RIGHT" : "LEFT", lastPos);
        }
    }

    printf("\n== Motor-command limiting ==\n");
    showClamp( 300,  -300, 255);  //  255, -255
    showClamp( 120,   -80, 255);  //  unchanged
    showClamp(-400,   400, 200);  // -200,  200
    showClamp( 255,  -255, 255);  //  at the limit, unchanged
    showClamp(   0,     0, 255);  //  zero stays zero

    CHECK(clampCommand( 300, 255) ==  255);
    CHECK(clampCommand(-300, 255) == -255);
    CHECK(clampCommand( 120, 255) ==  120);
    CHECK(clampCommand( -80, 255) ==  -80);
    CHECK(clampCommand( 255, 255) ==  255);
    CHECK(clampCommand(-255, 255) == -255);
    CHECK(clampCommand(   0, 255) ==    0);
    CHECK(clampCommand( 100,   0) ==    0);
    CHECK(clampCommand( 300,-255) ==  255);
    int a = 400, b = -400;
    clampMotorCommands(a, b, 200);
    CHECK(a == 200 && b == -200);

    if (failures == 0) printf("\nAll checks passed.\n");
    else               printf("\n%d check(s) FAILED.\n", failures);
    return failures == 0 ? 0 : 1;
}
