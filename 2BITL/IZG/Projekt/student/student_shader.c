/*!
 * @file
 * @brief This file contains implemenation of phong vertex and fragment shader.
 *
 * @author Tomáš Milet, imilet@fit.vutbr.cz
 */

#include <math.h>
#include <assert.h>
#include <stdio.h>

#include "student/student_shader.h"
#include "student/gpu.h"
#include "student/uniforms.h"

/// \addtogroup shader_side Úkoly v shaderech
/// @{

void phong_vertexShader(
        GPUVertexShaderOutput     *const output,
        GPUVertexShaderInput const*const input,
        GPU const gpu   ){
        /// \todo Naimplementujte vertex shader, který transformuje vstupní vrcholy do clip-space.<br>
        /// <b>Vstupy:</b><br>
        /// Vstupní vrchol by měl v nultém atributu obsahovat pozici vrcholu ve world-space (vec3) a v prvním
        /// atributu obsahovat normálu vrcholu ve world-space (vec3).<br>
        /// <b>Výstupy:</b><br>
        /// Výstupní vrchol by měl v nultém atributu obsahovat pozici vrcholu (vec3) ve world-space a v prvním
        /// atributu obsahovat normálu vrcholu ve world-space (vec3).
        /// Výstupní vrchol obsahuje pozici a normálu vrcholu proto, že chceme počítat osvětlení ve world-space ve fragment shaderu.<br>
        /// <b>Uniformy:</b><br>
        /// Vertex shader by měl pro transformaci využít uniformní proměnné obsahující view a projekční matici.
        /// View matici čtěte z uniformní proměnné "viewMatrix" a projekční matici čtěte z uniformní proměnné "projectionMatrix".
        /// Zachovejte jména uniformních proměnných a pozice vstupních a výstupních atributů.
        /// Pokud tak neučiníte, akceptační testy selžou.<br>
        /// <br>
        /// Využijte vektorové a maticové funkce.
        /// Nepředávajte si data do shaderu pomocí globálních proměnných.
        /// Pro získání dat atributů použijte příslušné funkce vs_interpret* definované v souboru program.h.
        /// Pro získání dat uniformních proměnných použijte příslušné funkce shader_interpretUniform* definované v souboru program.h.
        /// Vrchol v clip-space by měl být zapsán do proměnné gl_Position ve výstupní struktuře.<br>
        /// <b>Seznam funkcí, které jistě použijete</b>:
        ///  - gpu_getUniformsHandle()
        ///  - getUniformLocation()
        ///  - shader_interpretUniformAsMat4()
        ///  - vs_interpretInputVertexAttributeAsVec3()
        ///  - vs_interpretOutputVertexAttributeAsVec3()

        Uniforms const uniformsHandle = gpu_getUniformsHandle(gpu);

        Mat4 mvp;
        multiply_Mat4_Mat4(&mvp, shader_interpretUniformAsMat4(uniformsHandle, getUniformLocation(gpu, "projectionMatrix")) /* proj */,
                                      shader_interpretUniformAsMat4(uniformsHandle, getUniformLocation(gpu, "viewMatrix")) /* view */);

        Vec4 pos4;
        Vec3 const* in = vs_interpretInputVertexAttributeAsVec3(gpu, input, 0);
        copy_Vec3(vs_interpretOutputVertexAttributeAsVec3(gpu, output, 0), in);

        copy_Vec3Float_To_Vec4(&pos4, in, 1.f);
        multiply_Mat4_Vec4(&output->gl_Position, &mvp, &pos4);

        copy_Vec3(vs_interpretOutputVertexAttributeAsVec3(gpu, output, 1), vs_interpretInputVertexAttributeAsVec3(gpu, input, 1));
}

inline float powfi(float a, int b) {
      float n = 1.f;
      while (b) {
        if (b & 1) {
          n *= a;
        }
        a *= a;
        b >>= 1;
      }

      return n;
}

void phong_fragmentShader(
        GPUFragmentShaderOutput     *const output,
        GPUFragmentShaderInput const*const input,
        GPU const gpu   ){
        /// \todo Naimplementujte fragment shader, který počítá phongův osvětlovací model s phongovým stínováním.<br>
        /// <b>Vstup:</b><br>
        /// Vstupní fragment by měl v nultém fragment atributu obsahovat interpolovanou pozici ve world-space a v prvním
        /// fragment atributu obsahovat interpolovanou normálu ve world-space.<br>
        /// <b>Výstup:</b><br>
        /// Barvu zapište do proměnné color ve výstupní struktuře.<br>
        /// <b>Uniformy:</b><br>
        /// Pozici kamery přečtěte z uniformní proměnné "cameraPosition" a pozici světla přečtěte z uniformní proměnné "lightPosition".
        /// Zachovejte jména uniformních proměnný.
        /// Pokud tak neučiníte, akceptační testy selžou.<br>
        /// <br>
        /// Dejte si pozor na velikost normálového vektoru, při lineární interpolaci v rasterizaci může dojít ke zkrácení.
        /// Zapište barvu do proměnné color ve výstupní struktuře.
        /// Shininess faktor nastavte na 40.f
        /// Difuzní barvu materiálu nastavte na čistou zelenou.
        /// Spekulární barvu materiálu nastavte na čistou bílou.
        /// Barvu světla nastavte na bílou.
        /// Nepoužívejte ambientní světlo.<br>
        /// <b>Seznam funkcí, které jistě využijete</b>:
        ///  - shader_interpretUniformAsVec3()
        ///  - fs_interpretInputAttributeAsVec3()
        Uniforms const uniformsHandle = gpu_getUniformsHandle(gpu);

        Vec3 const* position = fs_interpretInputAttributeAsVec3(gpu, input, 0);

        Vec3 N;
        normalize_Vec3(&N, fs_interpretInputAttributeAsVec3(gpu, input, 1) /* normal */);

        Vec3 L;
        sub_Vec3(&L, shader_interpretUniformAsVec3(uniformsHandle, getUniformLocation(gpu, "lightPosition")) /* light */, position);
        normalize_Vec3(&L, &L);

        output->color.data[0] = output->color.data[1] = output->color.data[2] = 0.f;
        output->color.data[3] = 1.f;

        float ln = dot_Vec3(&L, &N);

        if (ln < 0.f) return;

        Vec3 V;
        sub_Vec3(&V, shader_interpretUniformAsVec3(uniformsHandle, getUniformLocation(gpu, "cameraPosition")) /* camera */, position);
        normalize_Vec3(&V, &V);
        Vec3 R;
        //reflect(&R, &L, &N);
        multiply_Vec3_Float(&N, &N, 2.f * ln);
        sub_Vec3(&R, &L, &N);

        float rv = dot_Vec3(&R, &V);

        if (rv < 0.f) {
              output->color.data[0] = output->color.data[1] = output->color.data[2] = powfi(rv, 40);
        }

        output->color.data[1] += ln;
}

/// @}
