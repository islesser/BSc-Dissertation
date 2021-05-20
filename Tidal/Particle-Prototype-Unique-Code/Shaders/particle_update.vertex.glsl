#version 410 core

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inVelocity;
layout (location = 2) in vec4 inColor;
layout (location = 3) in float inLifespan;
layout (location = 4) in float inMass;

out vec3 outPosition;
out vec3 outVelocity;
out vec4 outColor;
out float outLifespan;
out float outMass;

uniform float dt;
uniform samplerBuffer tbo_id0;

uniform vec2 minMaxSpeed;// x: Min, y: Max
uniform vec2 resolution;
uniform vec3 dimensions;
uniform float dragCoefficient;
uniform float gravity;
uniform int samples;
uniform float sampleStrength;
uniform float sampleStengthDegradation;

#define GD dimensions
#define SL minMaxSpeed
#define SIZE resolution

#define X GD.x
#define Y GD.y
#define Z GD.z

#define SIZE_X X * SIZE.x
#define SIZE_Y Y * SIZE.y
#define SIZE_Z Z * SIZE.x

#define MIN_SPEED SL.x
#define MAX_SPEED SL.y

vec3 CalculateDrag();
vec3 ApplyDrag(vec3 _drag, vec3 _velocity);
vec4 SampleForceTexture();
int SampleIndex(int x, int y, int z);
void CheckBounds();

// Map a value from one range to another.
float map(float value, float min1, float max1, float min2, float max2) {
    return min2 + (value - min1) * (max2 - min2) / (max1 - min1);
}

void main()
{
    // Only want to update if the particle is 'alive'
    if (inLifespan > 0.0)
    {
        // Get our force value.
        vec4 textureSample = SampleForceTexture();

        // Calculate the drag force that will act on the particle.
        vec3 drag = CalculateDrag();

        // Calculate gravity, want it to act more on higher particles.
        float gravityModifier = map(inPosition.y, 0.0, (SIZE_Y), 1.0, 5.0);

        // Calculate the particle velocity, limit to a flat plane in this instance (y = 0)
        vec3 calculatedVelocity = vec3(
            clamp(inVelocity.x + (textureSample.x * inMass), MIN_SPEED, MAX_SPEED) * dt,
            //clamp(inVelocity.y + (textureSample.y * inMass) - (gravity * gravityModifier * inMass), MIN_SPEED, MAX_SPEED) * dt,
            0.0,
            clamp(inVelocity.z + (textureSample.z * inMass), MIN_SPEED, MAX_SPEED) * dt
        );

        // Apply the drag to the particle.
        calculatedVelocity = ApplyDrag(drag, calculatedVelocity);

        // Apply the velocity to the position & check if particle is out of bounds and handle.
        outPosition = inPosition + calculatedVelocity;
        CheckBounds();

        // Assign a colour to the particle, based on it's current volocity.
        float r = map(inPosition.y, 0.0, 2 * SIZE_Y, 0.0, 1.0);
        float g = map(calculatedVelocity.x * 20, MIN_SPEED, MAX_SPEED, 0.0, 1.0);
        float b = map(calculatedVelocity.z * 20, MIN_SPEED, MAX_SPEED, 0.0, 1.0);
        outColor = vec4(r, g, b, outLifespan);

        // Passing remaining data
        outVelocity = calculatedVelocity;
        outMass = inMass;
    }
    else // It's dead... Simply pass values.
    {
        outPosition = inPosition;
        outColor = inColor;
        outLifespan = inLifespan;
        outMass = inMass;
        outVelocity = inVelocity;
    }
}

// Calculates the drag on a particle, adapted from NASA drag equation
// https://www.grc.nasa.gov/www/k-12/airplane/drageq.html
// D = Cd * ( (mass * velocity^2) / 2) * A
vec3 CalculateDrag()
{
    // Calc velocity
    float speed = length(inVelocity);
    // Cd * veocityl^2
    float dragSpeed = dragCoefficient * speed * speed;
    // Set the direction opposite to where the particle is moving
    vec3 drag = inVelocity * -1;
    // Calculation & return
    drag = normalize(drag) * dragSpeed * inMass;
    return drag;
}

// Applies drag to the passed velocity.
// Used to account for the ability of particles to move in 3D, we don't want
// particles to make them accelerate in opposite direction, only to bring them to
// a stop!
vec3 ApplyDrag(vec3 _drag, vec3 _velocity)
{
    vec3 newVelocity = _velocity;

    if (newVelocity.x >= 0 && newVelocity.x - _drag.x < 0.0) newVelocity.x = 0.0;
    else newVelocity.x -= _drag.x;

    if (newVelocity.y >= 0 && newVelocity.y - _drag.y < 0.0) newVelocity.y = 0.0;
    else newVelocity.y -= _drag.y;

    if (newVelocity.z >= 0 && newVelocity.z - _drag.z < 0.0) newVelocity.z = 0.0;
    else newVelocity.z -= _drag.z;

    return newVelocity;
}

// Samples the passed force grid texture at the particles current position.
// This will also sample in a radius around the particles position to apply
// additive forces, the radius is set from the 'samples' variable.
vec4 SampleForceTexture()
{
    vec4 textureSample = vec4(0.0, 0.0, 0.0, 0.0);
    float ss = sampleStrength;

    if (inPosition.y > 0)
    {
        textureSample += (texelFetch(tbo_id0, SampleIndex(0, 0, 0)));

        ss * sampleStengthDegradation;

        for (int i = 1; i <= samples; ++i)
        {
            textureSample += (texelFetch(tbo_id0, SampleIndex(0, 0, i)));   // Top
            textureSample += (texelFetch(tbo_id0, SampleIndex(-i, 0, 0)));  // Left
            textureSample += (texelFetch(tbo_id0, SampleIndex(i, 0, 0)));   // Right
            textureSample += (texelFetch(tbo_id0, SampleIndex(0, 0, -i)));  // Bottom

            textureSample += (texelFetch(tbo_id0, SampleIndex(i, 0, i)));   // Top Right
            textureSample += (texelFetch(tbo_id0, SampleIndex(-i, 0, i)));  // Top Left
            textureSample += (texelFetch(tbo_id0, SampleIndex(i, 0, -i)));   // Bottom Right
            textureSample += (texelFetch(tbo_id0, SampleIndex(-i, 0, -i)));  // Bottom Left

            ss *= sampleStengthDegradation;
        }
    }

    return textureSample * inMass;
}

// Helper function to calculate an index into the force
// texture from a particle position.
int SampleIndex(int x, int y, int z)
{
    int xp = int(int(inPosition.x + x) / SIZE_X);
    int yp = int(int((inPosition.y + y) / SIZE_Y) * Y);
    int zp = int(int((inPosition.z + z) / SIZE_Z) * Z);

    return yp + zp + xp;
}

// Checks the bounds to ensure particle is not escaping the simulation area.
// This implementation simply kills the particle.
void CheckBounds()
{
    if (outPosition.y < 0) outPosition.y = 1.0;

    if (outPosition.x > SIZE_X || outPosition.x < 0 ||
    outPosition.z > SIZE_Z || outPosition.z < 0)
    {
        outLifespan = 0.0f;
    }
    else
    {
        outLifespan = inLifespan;
    }
}
