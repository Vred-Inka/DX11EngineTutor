#include "GeometryGenerator.h"
#include "Graphics.h"

void GeometryGenerator::CreateBox(float width, float height, float depth, MeshData& meshData)
{
    //
    // Create the vertices.
    //

    Vertex3D v[24];

    float w2 = 0.5f*width;
    float h2 = 0.5f*height;
    float d2 = 0.5f*depth;

    // Fill in the front face Vertex3D data.
    v[0] = Vertex3D(-w2, -h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
    v[1] = Vertex3D(-w2, +h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
    v[2] = Vertex3D(+w2, +h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
    v[3] = Vertex3D(+w2, -h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    // Fill in the back face Vertex3D data.
    v[4] = Vertex3D(-w2, -h2, +d2, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f);
    v[5] = Vertex3D(+w2, -h2, +d2, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
    v[6] = Vertex3D(+w2, +h2, +d2, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
    v[7] = Vertex3D(-w2, +h2, +d2, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f);

    // Fill in the top face Vertex3D data.
    v[8] = Vertex3D(-w2, +h2, -d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
    v[9] = Vertex3D(-w2, +h2, +d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
    v[10] = Vertex3D(+w2, +h2, +d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
    v[11] = Vertex3D(+w2, +h2, -d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    // Fill in the bottom face Vertex3D data.
    v[12] = Vertex3D(-w2, -h2, -d2, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f);
    v[13] = Vertex3D(+w2, -h2, -d2, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
    v[14] = Vertex3D(+w2, -h2, +d2, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
    v[15] = Vertex3D(-w2, -h2, +d2, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f);

    // Fill in the left face Vertex3D data.
    v[16] = Vertex3D(-w2, -h2, +d2, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
    v[17] = Vertex3D(-w2, +h2, +d2, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
    v[18] = Vertex3D(-w2, +h2, -d2, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
    v[19] = Vertex3D(-w2, -h2, -d2, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);

    // Fill in the right face Vertex3D data.
    v[20] = Vertex3D(+w2, -h2, -d2, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f);
    v[21] = Vertex3D(+w2, +h2, -d2, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);
    v[22] = Vertex3D(+w2, +h2, +d2, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f);
    v[23] = Vertex3D(+w2, -h2, +d2, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);

    meshData.mVertices.assign(&v[0], &v[24]);

    //
    // Create the indices.
    //

    UINT i[36];

    // Fill in the front face index data
    i[0] = 0; i[1] = 1; i[2] = 2;
    i[3] = 0; i[4] = 2; i[5] = 3;

    // Fill in the back face index data
    i[6] = 4; i[7] = 5; i[8] = 6;
    i[9] = 4; i[10] = 6; i[11] = 7;

    // Fill in the top face index data
    i[12] = 8; i[13] = 9; i[14] = 10;
    i[15] = 8; i[16] = 10; i[17] = 11;

    // Fill in the bottom face index data
    i[18] = 12; i[19] = 13; i[20] = 14;
    i[21] = 12; i[22] = 14; i[23] = 15;

    // Fill in the left face index data
    i[24] = 16; i[25] = 17; i[26] = 18;
    i[27] = 16; i[28] = 18; i[29] = 19;

    // Fill in the right face index data
    i[30] = 20; i[31] = 21; i[32] = 22;
    i[33] = 20; i[34] = 22; i[35] = 23;

    meshData.mIndices.assign(&i[0], &i[36]);
}

void GeometryGenerator::CreateSphere(float radius, UINT sliceCount, UINT stackCount, MeshData& meshData)
{
    meshData.mVertices.clear();
    meshData.mIndices.clear();

    //
    // Compute the vertices stating at the top pole and moving down the stacks.
    //

    // Poles: note that there will be texture coordinate distortion as there is
    // not a unique point on the texture map to assign to the pole when mapping
    // a rectangular texture onto a sphere.
    Vertex3D topVertex3D(0.0f, +radius, 0.0f, 0.0f, +1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
    Vertex3D bottomVertex3D(0.0f, -radius, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);

    meshData.mVertices.push_back(topVertex3D);

    float phiStep = XM_PI / stackCount;
    float thetaStep = 2.0f*XM_PI / sliceCount;

    // Compute vertices for each stack ring (do not count the poles as rings).
    for (UINT i = 1; i <= stackCount - 1; ++i)
    {
        float phi = i * phiStep;

        // Vertices of ring.
        for (UINT j = 0; j <= sliceCount; ++j)
        {
            float theta = j * thetaStep;

            Vertex3D v;

            // spherical to cartesian
            v.mPos.x = radius * sinf(phi)*cosf(theta);
            v.mPos.y = radius * cosf(phi);
            v.mPos.z = radius * sinf(phi)*sinf(theta);

            // Partial derivative of P with respect to theta
            v.mTanU.x = -radius * sinf(phi)*sinf(theta);
            v.mTanU.y = 0.0f;
            v.mTanU.z = +radius * sinf(phi)*cosf(theta);

            XMVECTOR T = XMLoadFloat3(&v.mTanU);
            XMStoreFloat3(&v.mTanU, XMVector3Normalize(T));

            XMVECTOR p = XMLoadFloat3(&v.mPos);
            XMStoreFloat3(&v.mNormal, XMVector3Normalize(p));

            v.mTexCoord.x = theta / XM_2PI;
            v.mTexCoord.y = phi / XM_PI;

            meshData.mVertices.push_back(v);
        }
    }

    meshData.mVertices.push_back(bottomVertex3D);

    //
    // Compute indices for top stack.  The top stack was written first to the Vertex3D buffer
    // and connects the top pole to the first ring.
    //

    for (UINT i = 1; i <= sliceCount; ++i)
    {
        meshData.mIndices.push_back(0);
        meshData.mIndices.push_back(i + 1);
        meshData.mIndices.push_back(i);
    }

    //
    // Compute indices for inner stacks (not connected to poles).
    //

    // Offset the indices to the index of the first Vertex3D in the first ring.
    // This is just skipping the top pole vertex.
    UINT baseIndex = 1;
    UINT ringVertexCount = sliceCount + 1;
    for (UINT i = 0; i < stackCount - 2; ++i)
    {
        for (UINT j = 0; j < sliceCount; ++j)
        {
            meshData.mIndices.push_back(baseIndex + i * ringVertexCount + j);
            meshData.mIndices.push_back(baseIndex + i * ringVertexCount + j + 1);
            meshData.mIndices.push_back(baseIndex + (i + 1)*ringVertexCount + j);

            meshData.mIndices.push_back(baseIndex + (i + 1)*ringVertexCount + j);
            meshData.mIndices.push_back(baseIndex + i * ringVertexCount + j + 1);
            meshData.mIndices.push_back(baseIndex + (i + 1)*ringVertexCount + j + 1);
        }
    }

    //
    // Compute indices for bottom stack.  The bottom stack was written last to the Vertex3D buffer
    // and connects the bottom pole to the bottom ring.
    //

    // South pole Vertex3D was added last.
    UINT southPoleIndex = (UINT)meshData.mVertices.size() - 1;

    // Offset the indices to the index of the first Vertex3D in the last ring.
    baseIndex = southPoleIndex - ringVertexCount;

    for (UINT i = 0; i < sliceCount; ++i)
    {
        meshData.mIndices.push_back(southPoleIndex);
        meshData.mIndices.push_back(baseIndex + i);
        meshData.mIndices.push_back(baseIndex + i + 1);
    }
}

void GeometryGenerator::Subdivide(MeshData& meshData)
{
    // Save a copy of the input geometry.
    MeshData inputCopy = meshData;


    meshData.mVertices.resize(0);
    meshData.mIndices.resize(0);

    //       v1
    //       *
    //      / \
	//     /   \
	//  m0*-----*m1
    //   / \   / \
	//  /   \ /   \
	// *-----*-----*
    // v0    m2     v2

    UINT numTris = inputCopy.mIndices.size() / 3;
    for (UINT i = 0; i < numTris; ++i)
    {
        Vertex3D v0 = inputCopy.mVertices[inputCopy.mIndices[i * 3 + 0]];
        Vertex3D v1 = inputCopy.mVertices[inputCopy.mIndices[i * 3 + 1]];
        Vertex3D v2 = inputCopy.mVertices[inputCopy.mIndices[i * 3 + 2]];

        //
        // Generate the midpoints.
        //

        Vertex3D m0, m1, m2;

        // For subdivision, we just care about the position component.  We derive the other
        // Vertex3D components in CreateGeosphere.

        m0.mPos = XMFLOAT3(
            0.5f*(v0.mPos.x + v1.mPos.x),
            0.5f*(v0.mPos.y + v1.mPos.y),
            0.5f*(v0.mPos.z + v1.mPos.z));

        m1.mPos = XMFLOAT3(
            0.5f*(v1.mPos.x + v2.mPos.x),
            0.5f*(v1.mPos.y + v2.mPos.y),
            0.5f*(v1.mPos.z + v2.mPos.z));

        m2.mPos = XMFLOAT3(
            0.5f*(v0.mPos.x + v2.mPos.x),
            0.5f*(v0.mPos.y + v2.mPos.y),
            0.5f*(v0.mPos.z + v2.mPos.z));

        //
        // Add new geometry.
        //

        meshData.mVertices.push_back(v0); // 0
        meshData.mVertices.push_back(v1); // 1
        meshData.mVertices.push_back(v2); // 2
        meshData.mVertices.push_back(m0); // 3
        meshData.mVertices.push_back(m1); // 4
        meshData.mVertices.push_back(m2); // 5

        meshData.mIndices.push_back(i * 6 + 0);
        meshData.mIndices.push_back(i * 6 + 3);
        meshData.mIndices.push_back(i * 6 + 5);

        meshData.mIndices.push_back(i * 6 + 3);
        meshData.mIndices.push_back(i * 6 + 4);
        meshData.mIndices.push_back(i * 6 + 5);

        meshData.mIndices.push_back(i * 6 + 5);
        meshData.mIndices.push_back(i * 6 + 4);
        meshData.mIndices.push_back(i * 6 + 2);

        meshData.mIndices.push_back(i * 6 + 3);
        meshData.mIndices.push_back(i * 6 + 1);
        meshData.mIndices.push_back(i * 6 + 4);
    }
}

float GeometryGenerator::AngleFromXY(float x, float y)
{
    float theta = 0.0f;

    // Quadrant I or IV
    if (x >= 0.0f)
    {
        // If x = 0, then atanf(y/x) = +pi/2 if y > 0
        //                atanf(y/x) = -pi/2 if y < 0
        theta = atanf(y / x); // in [-pi/2, +pi/2]

        if (theta < 0.0f)
            theta += 2.0f*XM_PI; // in [0, 2*pi).
    }

    // Quadrant II or III
    else
        theta = atanf(y / x) + XM_PI; // in [0, 2*pi).

    return theta;
}

void GeometryGenerator::CreateGeosphere(float radius, UINT numSubdivisions, MeshData& meshData)
{
    // Put a cap on the number of subdivisions.
    numSubdivisions =std::min(numSubdivisions, 5u);

    // Approximate a sphere by tessellating an icosahedron.

    const float X = 0.525731f;
    const float Z = 0.850651f;

    XMFLOAT3 pos[12] =
    {
        XMFLOAT3(-X, 0.0f, Z),  XMFLOAT3(X, 0.0f, Z),
        XMFLOAT3(-X, 0.0f, -Z), XMFLOAT3(X, 0.0f, -Z),
        XMFLOAT3(0.0f, Z, X),   XMFLOAT3(0.0f, Z, -X),
        XMFLOAT3(0.0f, -Z, X),  XMFLOAT3(0.0f, -Z, -X),
        XMFLOAT3(Z, X, 0.0f),   XMFLOAT3(-Z, X, 0.0f),
        XMFLOAT3(Z, -X, 0.0f),  XMFLOAT3(-Z, -X, 0.0f)
    };

    DWORD k[60] =
    {
        1,4,0,  4,9,0,  4,5,9,  8,5,4,  1,8,4,
        1,10,8, 10,3,8, 8,3,5,  3,2,5,  3,7,2,
        3,10,7, 10,6,7, 6,11,7, 6,0,11, 6,1,0,
        10,1,6, 11,0,9, 2,11,9, 5,2,9,  11,2,7
    };

    meshData.mVertices.resize(12);
    meshData.mIndices.resize(60);

    for (UINT i = 0; i < 12; ++i)
        meshData.mVertices[i].mPos = pos[i];

    for (UINT i = 0; i < 60; ++i)
        meshData.mIndices[i] = k[i];

    for (UINT i = 0; i < numSubdivisions; ++i)
        Subdivide(meshData);

    // Project vertices onto sphere and scale.
    for (UINT i = 0; i < meshData.mVertices.size(); ++i)
    {
        // Project onto unit sphere.
        XMVECTOR n = XMVector3Normalize(XMLoadFloat3(&meshData.mVertices[i].mPos));

        // Project onto sphere.
        XMVECTOR p = radius * n;

        XMStoreFloat3(&meshData.mVertices[i].mPos, p);
        XMStoreFloat3(&meshData.mVertices[i].mNormal, n);

        // Derive texture coordinates from spherical coordinates.
        float theta = AngleFromXY(
            meshData.mVertices[i].mPos.x,
            meshData.mVertices[i].mPos.z);

        float phi = acosf(meshData.mVertices[i].mPos.y / radius);

        meshData.mVertices[i].mTexCoord.x = theta / XM_2PI;
        meshData.mVertices[i].mTexCoord.y = phi / XM_PI;

        // Partial derivative of P with respect to theta
        meshData.mVertices[i].mTanU.x = -radius * sinf(phi)*sinf(theta);
        meshData.mVertices[i].mTanU.y = 0.0f;
        meshData.mVertices[i].mTanU.z = +radius * sinf(phi)*cosf(theta);

        XMVECTOR T = XMLoadFloat3(&meshData.mVertices[i].mTanU);
        XMStoreFloat3(&meshData.mVertices[i].mTanU, XMVector3Normalize(T));
    }
}

void GeometryGenerator::CreateCylinder(float bottomRadius, float topRadius, float height, UINT sliceCount, UINT stackCount, MeshData& meshData)
{
    meshData.mVertices.clear();
    meshData.mIndices.clear();

    //
    // Build Stacks.
    // 

    float stackHeight = height / stackCount;

    // Amount to increment radius as we move up each stack level from bottom to top.
    float radiusStep = (topRadius - bottomRadius) / stackCount;

    UINT ringCount = stackCount + 1;

    // Compute vertices for each stack ring starting at the bottom and moving up.
    for (UINT i = 0; i < ringCount; ++i)
    {
        float y = -0.5f*height + i * stackHeight;
        float r = bottomRadius + i * radiusStep;

        // vertices of ring
        float dTheta = 2.0f*XM_PI / sliceCount;
        for (UINT j = 0; j <= sliceCount; ++j)
        {
            Vertex3D vertex;

            float c = cosf(j*dTheta);
            float s = sinf(j*dTheta);

            vertex.mPos = XMFLOAT3(r*c, y, r*s);

            vertex.mTexCoord.x = (float)j / sliceCount;
            vertex.mTexCoord.y = 1.0f - (float)i / stackCount;

            // Cylinder can be parameterized as follows, where we introduce v
            // parameter that goes in the same direction as the v tex-coord
            // so that the bitangent goes in the same direction as the v tex-coord.
            //   Let r0 be the bottom radius and let r1 be the top radius.
            //   y(v) = h - hv for v in [0,1].
            //   r(v) = r1 + (r0-r1)v
            //
            //   x(t, v) = r(v)*cos(t)
            //   y(t, v) = h - hv
            //   z(t, v) = r(v)*sin(t)
            // 
            //  dx/dt = -r(v)*sin(t)
            //  dy/dt = 0
            //  dz/dt = +r(v)*cos(t)
            //
            //  dx/dv = (r0-r1)*cos(t)
            //  dy/dv = -h
            //  dz/dv = (r0-r1)*sin(t)

            // This is unit length.
            vertex.mTanU = XMFLOAT3(-s, 0.0f, c);

            float dr = bottomRadius - topRadius;
            XMFLOAT3 bitangent(dr*c, -height, dr*s);

            XMVECTOR T = XMLoadFloat3(&vertex.mTanU);
            XMVECTOR B = XMLoadFloat3(&bitangent);
            XMVECTOR N = XMVector3Normalize(XMVector3Cross(T, B));
            XMStoreFloat3(&vertex.mNormal, N);

            meshData.mVertices.push_back(vertex);
        }
    }

    // Add one because we duplicate the first and last Vertex3D per ring
    // since the texture coordinates are different.
    UINT ringVertexCount = sliceCount + 1;

    // Compute indices for each stack.
    for (UINT i = 0; i < stackCount; ++i)
    {
        for (UINT j = 0; j < sliceCount; ++j)
        {
            meshData.mIndices.push_back(i*ringVertexCount + j);
            meshData.mIndices.push_back((i + 1)*ringVertexCount + j);
            meshData.mIndices.push_back((i + 1)*ringVertexCount + j + 1);

            meshData.mIndices.push_back(i*ringVertexCount + j);
            meshData.mIndices.push_back((i + 1)*ringVertexCount + j + 1);
            meshData.mIndices.push_back(i*ringVertexCount + j + 1);
        }
    }

    BuildCylinderTopCap(bottomRadius, topRadius, height, sliceCount, stackCount, meshData);
    BuildCylinderBottomCap(bottomRadius, topRadius, height, sliceCount, stackCount, meshData);
}

void GeometryGenerator::BuildCylinderTopCap(float bottomRadius, float topRadius, float height,
    UINT sliceCount, UINT stackCount, MeshData& meshData)
{
    UINT baseIndex = (UINT)meshData.mVertices.size();

    float y = 0.5f*height;
    float dTheta = 2.0f*XM_PI / sliceCount;

    // Duplicate cap ring vertices because the texture coordinates and normals differ.
    for (UINT i = 0; i <= sliceCount; ++i)
    {
        float x = topRadius * cosf(i*dTheta);
        float z = topRadius * sinf(i*dTheta);

        // Scale down by the height to try and make top cap texture coord area
        // proportional to base.
        float u = x / height + 0.5f;
        float v = z / height + 0.5f;

        meshData.mVertices.push_back(Vertex3D(x, y, z, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, u, v));
    }

    // Cap center vertex.
    meshData.mVertices.push_back(Vertex3D(0.0f, y, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.5f, 0.5f));

    // Index of center vertex.
    UINT centerIndex = (UINT)meshData.mVertices.size() - 1;

    for (UINT i = 0; i < sliceCount; ++i)
    {
        meshData.mIndices.push_back(centerIndex);
        meshData.mIndices.push_back(baseIndex + i + 1);
        meshData.mIndices.push_back(baseIndex + i);
    }
}

void GeometryGenerator::BuildCylinderBottomCap(float bottomRadius, float topRadius, float height,
    UINT sliceCount, UINT stackCount, MeshData& meshData)
{
    // 
    // Build bottom cap.
    //

    UINT baseIndex = (UINT)meshData.mVertices.size();
    float y = -0.5f*height;

    // vertices of ring
    float dTheta = 2.0f*XM_PI / sliceCount;
    for (UINT i = 0; i <= sliceCount; ++i)
    {
        float x = bottomRadius * cosf(i*dTheta);
        float z = bottomRadius * sinf(i*dTheta);

        // Scale down by the height to try and make top cap texture coord area
        // proportional to base.
        float u = x / height + 0.5f;
        float v = z / height + 0.5f;

        meshData.mVertices.push_back(Vertex3D(x, y, z, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, u, v));
    }

    // Cap center vertex.
    meshData.mVertices.push_back(Vertex3D(0.0f, y, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.5f, 0.5f));

    // Cache the index of center vertex.
    UINT centerIndex = (UINT)meshData.mVertices.size() - 1;

    for (UINT i = 0; i < sliceCount; ++i)
    {
        meshData.mIndices.push_back(centerIndex);
        meshData.mIndices.push_back(baseIndex + i);
        meshData.mIndices.push_back(baseIndex + i + 1);
    }
}

void GeometryGenerator::CreateGrid(float width, float depth, UINT m, UINT n, MeshData& meshData)
{
    UINT vertexCount = m * n;
    UINT faceCount = (m - 1)*(n - 1) * 2;

    //
    // Create the vertices.
    //

    float halfWidth = 0.5f*width;
    float halfDepth = 0.5f*depth;

    float dx = width / (n - 1);
    float dz = depth / (m - 1);

    float du = 1.0f / (n - 1);
    float dv = 1.0f / (m - 1);

    meshData.mVertices.resize(vertexCount);
    for (UINT i = 0; i < m; ++i)
    {
        float z = halfDepth - i * dz;
        for (UINT j = 0; j < n; ++j)
        {
            float x = -halfWidth + j * dx;

            meshData.mVertices[i*n + j].mPos = XMFLOAT3(x, 0.0f, z);
            meshData.mVertices[i*n + j].mNormal = XMFLOAT3(0.0f, 1.0f, 0.0f);
            meshData.mVertices[i*n + j].mTanU = XMFLOAT3(1.0f, 0.0f, 0.0f);

            // Stretch texture over grid.
            meshData.mVertices[i*n + j].mTexCoord.x = j * du;
            meshData.mVertices[i*n + j].mTexCoord.y = i * dv;
        }
    }

    //
    // Create the indices.
    //

    meshData.mIndices.resize(faceCount * 3); // 3 indices per face

    // Iterate over each quad and compute indices.
    UINT k = 0;
    for (UINT i = 0; i < m - 1; ++i)
    {
        for (UINT j = 0; j < n - 1; ++j)
        {
            meshData.mIndices[k] = i * n + j;
            meshData.mIndices[k + 1] = i * n + j + 1;
            meshData.mIndices[k + 2] = (i + 1)*n + j;

            meshData.mIndices[k + 3] = (i + 1)*n + j;
            meshData.mIndices[k + 4] = i * n + j + 1;
            meshData.mIndices[k + 5] = (i + 1)*n + j + 1;

            k += 6; // next quad
        }
    }
}

void GeometryGenerator::CreateFullscreenQuad(MeshData& meshData)
{
    meshData.mVertices.resize(4);
    meshData.mIndices.resize(6);

    // Position coordinates specified in NDC space.
    meshData.mVertices[0] = Vertex3D(
        -1.0f, -1.0f, 0.0f,
        0.0f, 0.0f, -1.0f,
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f);

    meshData.mVertices[1] = Vertex3D(
        -1.0f, +1.0f, 0.0f,
        0.0f, 0.0f, -1.0f,
        1.0f, 0.0f, 0.0f,
        0.0f, 0.0f);

    meshData.mVertices[2] = Vertex3D(
        +1.0f, +1.0f, 0.0f,
        0.0f, 0.0f, -1.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f);

    meshData.mVertices[3] = Vertex3D(
        +1.0f, -1.0f, 0.0f,
        0.0f, 0.0f, -1.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 1.0f);

    meshData.mIndices[0] = 0;
    meshData.mIndices[1] = 1;
    meshData.mIndices[2] = 2;

    meshData.mIndices[3] = 0;
    meshData.mIndices[4] = 2;
    meshData.mIndices[5] = 3;
}
