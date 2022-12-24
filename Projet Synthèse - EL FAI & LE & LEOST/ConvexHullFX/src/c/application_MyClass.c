#include "application_MyClass.h"
#include "algo.h"
#include "util.h"

JNIEXPORT void JNICALL Java_application_MyClass_findConvexHull(JNIEnv * env, jobject obj, jstring infilename, jstring outfilename, jint algo, jint sort) {
    const char *s1 = (*env)->GetStringUTFChars(env, infilename, NULL);
    const char *s2 = (*env)->GetStringUTFChars(env, outfilename, NULL);

    switch (algo) {
        case 1:
            SlowConvexHull(s1, s2);
            break;
        case 2:
            ConvexHull(s1, s2, sort);
            break;
        case 3:
            RapidConvexHull(s1, s2);
            break;
        default:
        break;
    }
}