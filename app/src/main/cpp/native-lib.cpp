#include <jni.h>
#include <string>
#include <math.h>
#include <iostream>
#include <Eigen/Dense>
#include <ceres/ceres.h>

using Eigen::MatrixXd;

extern "C" JNIEXPORT jstring JNICALL
Java_com_qyh_cerestest_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";


//    Eigen::MatrixXd m = Eigen::MatrixXd::Random(3, 3);
//    m = MatrixXd::Constant(3, 3, 1.2) * 50;

    return env->NewStringUTF(hello.c_str());
}




using namespace Eigen;

Vector3f vec;
Vector3f vec2;
Vector3f vecRtrn;

void vecLoad(float x, float y, float z, float x2, float y2, float z2) {
    vec(0) = x;
    vec(1) = y;
    vec(2) = z;
    vec2(0) = x2;
    vec2(1) = y2;
    vec2(2) = z2;
}

void vecAdd(Vector3f vecA, Vector3f vecB) {
    vecRtrn = vecA + vecB;
}

extern "C"
JNIEXPORT jfloatArray JNICALL
Java_com_qyh_cerestest_MainActivity_test(JNIEnv *env, jobject instance, jfloatArray array1_,
                                         jfloatArray array2_) {
    jfloatArray result;
    result = env->NewFloatArray(3);
    if (result == NULL) {
        return NULL; /* out of memory error thrown */
    }

    jfloat array1[3];
    jfloat* flt1 = env->GetFloatArrayElements( array1_,0);
    jfloat* flt2 = env->GetFloatArrayElements( array2_,0);


    vecLoad(flt1[0], flt1[1], flt1[2], flt2[0], flt2[1], flt2[2]);
    vecAdd(vec, vec2);

    array1[0] = vecRtrn[0];
    array1[1] = vecRtrn[1];
    array1[2] = vecRtrn[2];

    env->ReleaseFloatArrayElements(array1_, flt1, 0);
    env->ReleaseFloatArrayElements(array2_, flt2, 0);
    env->SetFloatArrayRegion(result, 0, 3, array1);
    return result;

}

using namespace ceres;

struct CostFunctor {
    template <typename T>
    bool operator()(const T* const x, T* residual) const {
        residual[0] = T(10.0) - x[0];
        return true;
    }
};

extern "C"
JNIEXPORT jstring JNICALL
Java_com_qyh_cerestest_MainActivity_testCeres(JNIEnv *env, jobject instance) {
    std::string hello = "Hello Ceres";

    // 寻优参数x的初始值，为5
    double initial_x = 5.0;
    double x = initial_x;

    // 第二部分：构建寻优问题
    Problem problem;
    CostFunction* cost_function =
            new AutoDiffCostFunction<CostFunctor, 1, 1>(new CostFunctor); //使用自动求导，将之前的代价函数结构体传入，第一个1是输出维度，即残差的维度，第二个1是输入维度，即待寻优参数x的维度。
    problem.AddResidualBlock(cost_function, NULL, &x); //向问题中添加误差项，本问题比较简单，添加一个就行。

    //第三部分： 配置并运行求解器
    Solver::Options options;
    options.linear_solver_type = ceres::DENSE_QR; //配置增量方程的解法
    options.minimizer_progress_to_stdout = true;//输出到cout
    Solver::Summary summary;//优化信息
    ceres::Solve(options, &problem, &summary);//求解!!!

    std::cout << summary.BriefReport() << "\n";//输出优化的简要信息
    //最终结果
    std::cout << "x : " << initial_x
              << " -> " << x << "\n";

    return env->NewStringUTF(summary.BriefReport().c_str());
}