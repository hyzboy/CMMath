#include<hgl/graph/OBB.h>

namespace hgl::graph
{
    void OBB::SetFromPointsMinVolumeFloat(const float *points,size_t count,uint32_t component_count,float coarseStepDeg,float fineStepDeg,float ultraStepDeg)
    {
        if(points==nullptr||count==0) { this->Clear(); return; }
        if(count==1) {
            center=glm::vec3(points[0],points[1],points[2]);
            axis[0]=Vector3f(1,0,0);
            axis[1]=Vector3f(0,1,0);
            axis[2]=Vector3f(0,0,1);
            half_length=Vector3f(0,0,0);
            ComputePlanes();
            return;
        }

        std::vector<float> xs(count),ys(count),zs(count);
    #ifdef _OPENMP
    #pragma omp parallel for if(count > 1024)
    #endif
        for(int i=0; i<static_cast<int>(count); ++i)
        {
            const float *p=points+i*component_count;
            xs[i]=p[0];
            ys[i]=p[1];
            zs[i]=p[2];
        }

        auto makeR=[](float yawDeg,float pitchDeg,float rollDeg)
            {
                const float k=3.14159265358979323846f/180.0f;
                float yaw=yawDeg*k,pitch=pitchDeg*k,roll=rollDeg*k;
                glm::quat qz=glm::angleAxis(yaw,glm::vec3(0,0,1));
                glm::quat qy=glm::angleAxis(pitch,glm::vec3(0,1,0));
                glm::quat qx=glm::angleAxis(roll,glm::vec3(1,0,0));
                glm::quat q=qz*qy*qx;
                return glm::mat3_cast(q);
            };

        auto evalOrientation=[&](const glm::mat3 &R,OBB &out)
            {
                const glm::vec3 U=glm::vec3(R[0]);
                const glm::vec3 V=glm::vec3(R[1]);
                const glm::vec3 W=glm::vec3(R[2]);
                float minU=std::numeric_limits<float>::infinity(),maxU=-minU;
                float minV=std::numeric_limits<float>::infinity(),maxV=-minV;
                float minW=std::numeric_limits<float>::infinity(),maxW=-minW;

            #if defined(__AVX2__)
                const size_t N=count-(count%8);
                __m256 Ux=_mm256_set1_ps(U.x); __m256 Uy=_mm256_set1_ps(U.y); __m256 Uz=_mm256_set1_ps(U.z);
                __m256 Vx=_mm256_set1_ps(V.x); __m256 Vy=_mm256_set1_ps(V.y); __m256 Vz=_mm256_set1_ps(V.z);
                __m256 Wx=_mm256_set1_ps(W.x); __m256 Wy=_mm256_set1_ps(W.y); __m256 Wz=_mm256_set1_ps(W.z);

                __m256 minU8=_mm256_set1_ps(std::numeric_limits<float>::infinity());
                __m256 maxU8=_mm256_set1_ps(-std::numeric_limits<float>::infinity());
                __m256 minV8=minU8,maxV8=maxU8;
                __m256 minW8=minU8,maxW8=maxU8;

                for(size_t i=0;i<N;i+=8)
                {
                    __m256 X=_mm256_loadu_ps(xs.data()+i);
                    __m256 Y=_mm256_loadu_ps(ys.data()+i);
                    __m256 Z=_mm256_loadu_ps(zs.data()+i);
                    __m256 pu=_mm256_fmadd_ps(Uz,Z,_mm256_fmadd_ps(Uy,Y,_mm256_mul_ps(Ux,X)));
                    __m256 pv=_mm256_fmadd_ps(Vz,Z,_mm256_fmadd_ps(Vy,Y,_mm256_mul_ps(Vx,X)));
                    __m256 pw=_mm256_fmadd_ps(Wz,Z,_mm256_fmadd_ps(Wy,Y,_mm256_mul_ps(Wx,X)));
                    minU8=_mm256_min_ps(minU8,pu); maxU8=_mm256_max_ps(maxU8,pu);
                    minV8=_mm256_min_ps(minV8,pv); maxV8=_mm256_max_ps(maxV8,pv);
                    minW8=_mm256_min_ps(minW8,pw); maxW8=_mm256_max_ps(maxW8,pw);
                }
                alignas(32) float buMin[8],buMax[8],bvMin[8],bvMax[8],bwMin[8],bwMax[8];
                _mm256_store_ps(buMin,minU8); _mm256_store_ps(buMax,maxU8);
                _mm256_store_ps(bvMin,minV8); _mm256_store_ps(bvMax,maxV8);
                _mm256_store_ps(bwMin,minW8); _mm256_store_ps(bwMax,maxW8);
                for(int k=0;k<8;++k)
                {
                    if(buMin[k]<minU) minU=buMin[k]; if(buMax[k]>maxU) maxU=buMax[k];
                    if(bvMin[k]<minV) minV=bvMin[k]; if(bvMax[k]>maxV) maxV=bvMax[k];
                    if(bwMin[k]<minW) minW=bwMin[k]; if(bwMax[k]>maxW) maxW=bwMax[k];
                }
                for(size_t i=N;i<count;++i)
                {
                    float x=xs[i],y=ys[i],z=zs[i];
                    float pu=x*U.x+y*U.y+z*U.z; if(pu<minU) minU=pu; if(pu>maxU) maxU=pu;
                    float pv=x*V.x+y*V.y+z*V.z; if(pv<minV) minV=pv; if(pv>maxV) maxV=pv;
                    float pw=x*W.x+y*W.y+z*W.z; if(pw<minW) minW=pw; if(pw>maxW) maxW=pw;
                }
            #else
                for(size_t i=0;i<count;++i)
                {
                    float x=xs[i],y=ys[i],z=zs[i];
                    float pu=x*U.x+y*U.y+z*U.z; if(pu<minU) minU=pu; if(pu>maxU) maxU=pu;
                    float pv=x*V.x+y*V.y+z*V.z; if(pv<minV) minV=pv; if(pv>maxV) maxV=pv;
                    float pw=x*W.x+y*W.y+z*W.z; if(pw<minW) minW=pw; if(pw>maxW) maxW=pw;
                }
            #endif
                float sx=maxU-minU,sy=maxV-minV,sz=maxW-minW;
                float volume=sx*sy*sz;
                out.center=U*(0.5f*(minU+maxU))+V*(0.5f*(minV+maxV))+W*(0.5f*(minW+maxW));
                out.axis[0]=U; out.axis[1]=V; out.axis[2]=W;
                out.half_length=Vector3f(0.5f*sx,0.5f*sy,0.5f*sz);
                out.ComputePlanes();
                return volume;
            };

        int yawSteps=static_cast<int>(360.0f/coarseStepDeg)+1;
        int pitchSteps=static_cast<int>(180.0f/coarseStepDeg)+1;
        int rollSteps=static_cast<int>(360.0f/coarseStepDeg)+1;
        float bestYaw=0,bestPitch=0,bestRoll=0; float bestVol=std::numeric_limits<float>::infinity();
        OBB bestTmp,tmp;

    #ifdef _OPENMP
    #pragma omp parallel
        {
            OBB localBestBox; OBB localTmp;
            float localBestVol=std::numeric_limits<float>::infinity();
            float localYaw=0,localPitch=0,localRoll=0;
            int total=yawSteps*pitchSteps*rollSteps;
        #pragma omp for schedule(static)
            for(int idx=0; idx<total; ++idx)
            {
                int block=pitchSteps*rollSteps;
                int iyaw=idx/block;
                int rem=idx-iyaw*block;
                int ipitch=rem/rollSteps;
                int iroll=rem-ipitch*rollSteps;
                float yaw=-180.0f+iyaw*coarseStepDeg;
                float pitch=-90.0f+ipitch*coarseStepDeg;
                float roll=-180.0f+iroll*coarseStepDeg;
                glm::mat3 R=makeR(yaw,pitch,roll);
                float vol=evalOrientation(R,localTmp);
                if(vol<localBestVol) { localBestVol=vol; localBestBox=localTmp; localYaw=yaw; localPitch=pitch; localRoll=roll; }
            }
        #pragma omp critical
            {
                if(localBestVol<bestVol) { bestVol=localBestVol; bestTmp=localBestBox; bestYaw=localYaw; bestPitch=localPitch; bestRoll=localRoll; }
            }
        }
    #else
        for(int iyaw=0; iyaw<yawSteps; ++iyaw)
        {
            float yaw=-180.0f+iyaw*coarseStepDeg;
            for(int ipitch=0; ipitch<pitchSteps; ++ipitch)
            {
                float pitch=-90.0f+ipitch*coarseStepDeg;
                for(int iroll=0; iroll<rollSteps; ++iroll)
                {
                    float roll=-180.0f+iroll*coarseStepDeg;
                    glm::mat3 R=makeR(yaw,pitch,roll);
                    float vol=evalOrientation(R,tmp);
                    if(vol<bestVol) { bestVol=vol; bestTmp=tmp; bestYaw=yaw; bestPitch=pitch; bestRoll=roll; }
                }
            }
        }
    #endif
        OBB best=bestTmp;

        auto refine=[&](float rangeDeg,float stepDeg)
            {
                float y0=bestYaw,p0=bestPitch,r0=bestRoll;
                int steps=static_cast<int>((2*rangeDeg)/stepDeg)+1;
            #ifdef _OPENMP
            #pragma omp parallel
                {
                    OBB localBest=best; float localBestVol=bestVol;
                    float localYaw=y0,localPitch=p0,localRoll=r0;
                    int total=steps*steps*steps;
                #pragma omp for schedule(static)
                for(int idx=0; idx<total; ++idx)
                {
                    int plane=steps*steps;
                    int iy=idx/plane; int rem=idx-iy*plane;
                    int ip=rem/steps; int ir=rem-ip*steps;
                    float dy=-rangeDeg+iy*stepDeg;
                    float dp=-rangeDeg+ip*stepDeg;
                    float dr=-rangeDeg+ir*stepDeg;
                    glm::mat3 R=makeR(y0+dy,p0+dp,r0+dr);
                    OBB tmpLocal; float vol=evalOrientation(R,tmpLocal);
                    if(vol<localBestVol) { localBestVol=vol; localBest=tmpLocal; localYaw=y0+dy; localPitch=p0+dp; localRoll=r0+dr; }
                }
            #pragma omp critical
                {
                    if(localBestVol<bestVol) { bestVol=localBestVol; best=localBest; bestYaw=localYaw; bestPitch=localPitch; bestRoll=localRoll; }
                }
            }
            #else
                for(float dy=-rangeDeg; dy<=rangeDeg; dy+=stepDeg)
                {
                    for(float dp=-rangeDeg; dp<=rangeDeg; dp+=stepDeg)
                    {
                        for(float dr=-rangeDeg; dr<=rangeDeg; dr+=stepDeg)
                        {
                            glm::mat3 R=makeR(y0+dy,p0+dp,r0+dr);
                            float vol=evalOrientation(R,tmp);
                            if(vol<bestVol) { bestVol=vol; best=tmp; bestYaw=y0+dy; bestPitch=p0+dp; bestRoll=r0+dr; }
                        }
                    }
                }
            #endif
            };

        refine(15.0f,fineStepDeg);
        refine(3.0f,ultraStepDeg);
        *this = best;
    }

    void OBB::SetFromPoints(const float *points,size_t count,uint32_t component_count,float coarseStepDeg,float fineStepDeg,float ultraStepDeg)
    {
        SetFromPointsMinVolumeFloat(points,count,component_count,coarseStepDeg,fineStepDeg,ultraStepDeg);
    }
}//namespace hgl::graph
