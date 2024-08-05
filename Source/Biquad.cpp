/*
  ==============================================================================

    Biquad.cpp
    Created: 10 Jun 2024 11:35:49am
    Author:  Zachary Miller

  ==============================================================================
*/

#include "Biquad.h"
#include <cmath>

float Biquad::processSample(float x, int channel)
{
    float y;
    switch (filterQuality) {
        
        case Standard:
        y = (b0 / a0) * x + (b1 / a0) * x1[channel] + (b2 / a0) * x2[channel]
            + (-a1 / a0) * y1[channel] + (-a2 / a0) * y2[channel];
            break;
            
        case HiFi:
        y = b0 * x + b1 * x1[channel] + b2 * x2[channel]
            + (-a1) * y1[channel] + (-a2) * y2[channel];
            break;
    }
    
    x2[channel] = x1[channel]; // store delay samples for next process step
    x1[channel] = x;
    y2[channel] = y1[channel];
    y1[channel] = y;

    return y;
};

void Biquad::setFs(float sampleRate)
{
    this->Fs = sampleRate;
    updateCoefficients(); // Need to update if Fs changes
};

void Biquad::setFreq(double freq)
{
    
    this->freq = freq;

    // Use this to adjust for Bilinear Transform:
    //fc = (2.0f*Fs) * std::tan(fc/(Fs*2.0f)); //Frequency Warping

    w0 = (2.0 * M_PI) * freq / Fs; // Normalize frequency

    // Bandwidth/slope/resonance parameter
    alpha = std::sin(w0) / (2.0 * Q);

    updateCoefficients();
};

void Biquad::setQ(double Q)
{
    this->Q = Q;

    // Bandwidth/slope/resonance parameter
    alpha = std::sin(w0) / (2.0 * Q);

    updateCoefficients();
};

void Biquad::setAmpdB(double ampdB)
{
    this->ampdB = ampdB;

    A = std::pow(10.0, ampdB / 40.0); // Linear amplitude

    updateCoefficients();
};

void Biquad::updateCoefficients()
{
    switch (filterQuality)
    {
        case Standard: // RBJ biquads
            
            switch (filterType)
            {
                case LPF:
                    b0 = (1.0f - std::cos(w0)) / 2.0f;
                    b1 = 1.0f - std::cos(w0);
                    b2 = (1.0f - std::cos(w0)) / 2.0f;
                    a0 = 1.0f + alpha;
                    a1 = -2.0f * cos(w0);
                    a2 = 1.0f - alpha;
                    break;
                    
                case HPF:
                    b0 = (1.0f + std::cos(w0)) / 2.0f;
                    b1 = -(1.0f + std::cos(w0));
                    b2 = (1.0f + std::cos(w0)) / 2.0f;
                    a0 = 1.0f + alpha;
                    a1 = -2.0f * std::cos(w0);
                    a2 = 1.0f - alpha;
                    break;
                    
                case BPF: // BPF1 in RBJ Cookbook
                    b0 = std::sin(w0) / 2.0f;
                    b1 = 0.0f;
                    b2 = -std::sin(w0) / 2.0f;
                    a0 = 1.0f + alpha;
                    a1 = -2.0f * std::cos(w0);
                    a2 = 1.0f - alpha;
                    break;
                                        
                case PEAK:
                    b0 = 1.0f + alpha * A;
                    b1 = -2.0f * std::cos(w0);
                    b2 = 1.0f - alpha * A;
                    a0 = 1.0f + alpha / A;
                    a1 = -2.0f * std::cos(w0);
                    a2 = 1.0f - alpha / A;
                    break;
                    
                case NOTCH:
                    b0 = 1.0f;
                    b1 = -2.0f * std::cos(w0);
                    b2 = 1.0f;
                    a0 = 1.0f + alpha;
                    a1 = -2.0f * std::cos(w0);
                    a2 = 1.0f - alpha;
                    break;
                    
                case APF:
                    b0 = 1.0f - alpha;
                    b1 = -2.0f * std::cos(w0);
                    b2 = 1.0f + alpha;
                    a0 = 1.0f + alpha;
                    a1 = -2.0f * std::cos(w0);
                    a2 = 1.0f - alpha;
                    break;
                    
                case LSHELF:
                    b0 = A * ((A + 1.0f) - (A - 1.0f) * std::cos(w0) + 2.0f * std::sqrt(A) * alpha);
                    b1 = 2.0f * A * ((A - 1.0f) - (A + 1.0f) * std::cos(w0));
                    b2 = A * ((A + 1.0f) - (A - 1.0f) * std::cos(w0) - 2.0f * std::sqrt(A) * alpha);
                    a0 = (A + 1.0f) + (A - 1.0f) * std::cos(w0) + 2.0f * std::sqrt(A) * alpha;
                    a1 = -2.0f * ((A - 1.0f) + (A + 1.0f) * std::cos(w0));
                    a2 = (A + 1.0f) + (A - 1.0f) * std::cos(w0) - 2.0f * std::sqrt(A) * alpha;
                    break;
                    
                case HSHELF:
                    b0 = A * ((A + 1.0f) + (A - 1.0f) * std::cos(w0) + 2.0f * std::sqrt(A) * alpha);
                    b1 = -2.0f * A * ((A - 1.0f) + (A + 1.0f) * std::cos(w0));
                    b2 = A * ((A + 1.0f) + (A - 1.0f) * std::cos(w0) - 2.0f * std::sqrt(A) * alpha);
                    a0 = (A + 1.0f) - (A - 1.0f) * std::cos(w0) + 2.0f * std::sqrt(A) * alpha;
                    a1 = 2.0f * ((A - 1.0f) - (A + 1.0f) * std::cos(w0));
                    a2 = (A + 1.0f) - (A - 1.0f) * std::cos(w0) - 2.0f * std::sqrt(A) * alpha;
                    break;
                    
                default: // LPF
                    b0 = (1.0f - std::cos(w0)) / 2.0f;
                    b1 = 1.0f - std::cos(w0);
                    b2 = (1.0f - std::cos(w0)) / 2.0f;
                    a0 = 1.0f + alpha;
                    a1 = -2.0f * std::cos(w0);
                    a2 = 1.0f - alpha;
                    break;
            }
            
            break;
            
        case HiFi: // Matched (Vicanek) biquads
            
                        
            switch (filterType)
            
            {
                case LPF:
                    
                    // Base variables
                    sinrad2 = std::sin(w0/2);
                    q = 1/(2*Q);
                    
                    if (q <= 1) {
                        a1 = (-2 * exp(-q*w0)) * std::cos(std::sqrt(1-(std::pow(q,2.0)))*w0);
                    } else {
                        a1 = (-2 * exp(-q*w0)) * std::cosh(std::sqrt((std::pow(q,2.0))-1)*w0);
                    }
                    
                    a2 = exp(-2 * q * w0);
                    
                    phi0 = 1 - std::pow(sinrad2,2.0);
                    phi1 = std::pow(sinrad2,2.0);
                    phi2 = 4 * phi0 * phi1;
                                
                    A0 = 1 + a1 + a2;
                    A1 = 1 - a1 + a2;
                    A2 = -4 * a2;
                    
                    A0 *= A0; A1 *= A1;
                    
                    // LPF
                    R1 = (A0*phi0 + A1*phi1 + A2*phi2) * std::pow(Q,2.0);
                    B0 = A0;
                    B1 = (R1 - B0*phi0) / phi1;
                    
                    b0 = 0.5 * (std::sqrt(B0) + std::sqrt(B1));
                    b1 = std::sqrt(B0) - b0;
                    b2 = 0.0;

                    break;
                    
                case HPF:
                    
                    // Base variables
                    sinrad2 = std::sin(w0/2);
                    q = 1/(2*Q);
                    
                    if (q <= 1) {
                        a1 = (-2 * exp(-q*w0)) * std::cos(std::sqrt(1-(std::pow(q,2.0)))*w0);
                    } else {
                        a1 = (-2 * exp(-q*w0)) * std::cosh(std::sqrt((std::pow(q,2.0))-1)*w0);
                    }
                    
                    a2 = exp(-2 * q * w0);
                    
                    phi0 = 1 - std::pow(sinrad2,2.0);
                    phi1 = std::pow(sinrad2,2.0);
                    phi2 = 4 * phi0 * phi1;
                                
                    A0 = 1 + a1 + a2;
                    A1 = 1 - a1 + a2;
                    A2 = -4 * a2;
                    
                    A0 *= A0; A1 *= A1;
                    
                    // HPF
                    b2 = b0 = (std::sqrt(A0*phi0 + A1*phi1 + A2*phi2) * Q) / (4*phi1);
                    b1 = -2 * b0;
                    
                    break;
                    
                case BPF:
                    
                    // Base variables
                    sinrad2 = std::sin(w0/2);
                    q = 1/(2*Q);
                    
                    if (q <= 1) {
                        a1 = (-2 * exp(-q*w0)) * std::cos(std::sqrt(1-(std::pow(q,2.0)))*w0);
                    } else {
                        a1 = (-2 * exp(-q*w0)) * std::cosh(std::sqrt((std::pow(q,2.0))-1)*w0);
                    }
                    
                    a2 = exp(-2 * q * w0);
                    
                    phi0 = 1 - std::pow(sinrad2,2.0);
                    phi1 = std::pow(sinrad2,2.0);
                    phi2 = 4 * phi0 * phi1;
                                
                    A0 = 1 + a1 + a2;
                    A1 = 1 - a1 + a2;
                    A2 = -4 * a2;
                    
                    A0 *= A0; A1 *= A1;
                    
                    // BPF
                    R1 = A0*phi0 + A1*phi1 + A2*phi2;
                    R2 = -A0 + A1 + 4 * (phi0 - phi1) * A2;
                    B0 = 0;
                    B2 = (R1 - R2 * phi1) / (4 * std::pow(phi1,2));
                    B1 = R2 + 4 * (phi1 - phi0) * B2;
                    
                    b1 = (-0.5) * std::sqrt(B1);
                    b0 = 0.5 * (std::sqrt(B2 + std::pow(b1,2)) - b1);
                    b2 = -b0 - b1;

                    break;
                    
                case PEAK:
                    
                    // Base variables
                    sinrad2 = std::sin(w0/2);
                    q = 1/(2*Q);
                    
                    if (q <= 1) {
                        a1 = (-2 * exp(-q*w0)) * std::cos(std::sqrt(1-(std::pow(q,2.0)))*w0);
                    } else {
                        a1 = (-2 * exp(-q*w0)) * std::cosh(std::sqrt((std::pow(q,2.0))-1)*w0);
                    }
                    
                    a2 = exp(-2 * q * w0);
                    
                    phi0 = 1 - std::pow(sinrad2,2.0);
                    phi1 = std::pow(sinrad2,2.0);
                    phi2 = 4 * phi0 * phi1;
                                
                    A0 = 1 + a1 + a2;
                    A1 = 1 - a1 + a2;
                    A2 = -4 * a2;
                    
                    A0 *= A0; A1 *= A1;
                    
                    G4 = std::pow(A,4.001);
                    R1 = (A0*phi0 + A1*phi1 + A2*phi2)*G4;
                    R2 = (-A0 + A1 + 4*(phi0 - phi1)*A2)*G4;
                    B0 = A0;
                    B2 = (R1 - R2*phi1 - B0)/(4*phi1*phi1);
                    B1 = R2 + B0 + 4*(phi1 - phi0)*B2;
                    W  = 0.5 * (std::sqrt(B0) + std::sqrt(B1));
                    b0 = 0.5 * (W + sqrt(std::pow(W,2.0) + B2));
                    b1 = 0.5 * (std::sqrt(B0) - std::sqrt(B1));
                    b2 = -B2/(4*b0);
                    
                    break;
                    
                case NOTCH:
                    
                    // Base variables
                    sinrad2 = std::sin(w0/2);
                    q = 1/(2*Q);
                    
                    if (q <= 1) {
                        a1 = (-2 * exp(-q*w0)) * std::cos(std::sqrt(1-(std::pow(q,2.0)))*w0);
                    } else {
                        a1 = (-2 * exp(-q*w0)) * std::cosh(std::sqrt((std::pow(q,2.0))-1)*w0);
                    }
                    
                    a2 = exp(-2 * q * w0);
                    
                    phi0 = 1 - std::pow(sinrad2,2.0);
                    phi1 = std::pow(sinrad2,2.0);
                    phi2 = 4 * phi0 * phi1;
                                
                    A0 = 1 + a1 + a2;
                    A1 = 1 - a1 + a2;
                    A2 = -4 * a2;
                    
                    A0 *= A0; A1 *= A1;
                    
                    b0 = 1;
                    b1 = db1 = -2 * std::cos(w0);
                    b2 = 1;
                    
                    scale = std::sqrt(A0) / (b0 + db1 + b2);
                    b0 *= scale;
                    b1 *= scale;
                    b2 *= scale;
                    
                    break;
                                        
                case LSHELF:
                                                            
                    w0 = freq / (Fs/2);
                                    
                    // Special case gain
                    if (abs(1 - A) < 1e-6)
                        shlvG = 1.00001;
                    else
                        shlvG = 1/A;
                        
                    invG = 1 / shlvG;
                        
                    fc4 = std::pow(w0,4.0);
                    hny = (fc4 + shlvG) / (fc4 + invG); // magnitude sqrd at Nyq
                        
                    f1 = w0 / (sqrt(0.160 + 1.543 * std::pow(w0,2.0)));
                    f14 = std::pow(f1,4.0);
                    h1 = (fc4 + f14*shlvG) / (fc4 + f14*invG);
                    phi1 = std::sin(M_PI_2 * f1); phi1 *= phi1;
                        
                    f2 = w0 / std::sqrt(0.947 + 3.806 * std::pow(w0,2.0));
                    f24 = std::pow(f2,4.0);
                    h2 = (fc4 + f24*shlvG) / (fc4 + f24*invG);
                    phi2 = std::sin(M_PI_2 * f2); phi2 *= phi2;
                        
                    d1 = (h1 - 1.0) * (1.0 - phi1);
                    c11 = -phi1 * d1;
                    c12 = std::pow(phi1,2.0) * (hny - h1);
                        
                    d2 = (h2 - 1.0) * (1.0 - phi2);
                    c21 = -phi2 * d2;
                    c22 = std::pow(phi2,2.0) * (hny - h2);
                        
                    alpha1 = (c22*d1 - c12*d2) / (c11*c22 - c12*c21);
                    aa1 = (d1 - c11*alpha1) / c12;
                    bb1 = hny * aa1;
                        
                    aa2 = 0.25 * (alpha1 - aa1);
                    bb2 = 0.25 * (alpha1 - bb1);
                        
                    v = 0.5 * (1 + std::sqrt(aa1));
                    w = 0.5 * (1 + std::sqrt(bb1));
                    a0 = 0.5 * (v + sqrt(std::pow(v,2.0) + aa2));
                    inva0 = 1/a0;
                        
                    a1 = (1.0 - v) * inva0;
                    a2 = -0.25 * aa2 * std::pow(inva0,2.0);
                        
                    b0 = ((0.5 * (w + sqrt(std::pow(w,2.0) + bb2))) * inva0);
                    b1 = ((1 - w) * inva0);
                    b2 = ((-0.25 * bb2 / b0) * std::pow(inva0,2.0));

                    b0 *= A; b1 *= A; b2 *= A;
                                        
                    break;
                    
                case HSHELF:
                           
                    w0 = freq / (Fs/2);
                                    
                    // Special case gain
                    if (abs(1 - A) < 1e-6)
                        shlvG = 1.00001;
                    else
                        shlvG = A;
                        
                    invG = 1 / shlvG;
                        
                    fc4 = std::pow(w0,4.0);
                    hny = (fc4 + shlvG) / (fc4 + invG); // magnitude sqrd at Nyq
                        
                    f1 = w0 / (sqrt(0.160 + 1.543 * std::pow(w0,2.0)));
                    f14 = std::pow(f1,4.0);
                    h1 = (fc4 + f14*shlvG) / (fc4 + f14*invG);
                    phi1 = std::sin(M_PI_2 * f1); phi1 *= phi1;
                        
                    f2 = w0 / std::sqrt(0.947 + 3.806 * std::pow(w0,2.0));
                    f24 = std::pow(f2,4.0);
                    h2 = (fc4 + f24*shlvG) / (fc4 + f24*invG);
                    phi2 = std::sin(M_PI_2 * f2); phi2 *= phi2;
                        
                    d1 = (h1 - 1.0) * (1.0 - phi1);
                    c11 = -phi1 * d1;
                    c12 = std::pow(phi1,2.0) * (hny - h1);
                        
                    d2 = (h2 - 1.0) * (1.0 - phi2);
                    c21 = -phi2 * d2;
                    c22 = std::pow(phi2,2.0) * (hny - h2);
                        
                    alpha1 = (c22*d1 - c12*d2) / (c11*c22 - c12*c21);
                    aa1 = (d1 - c11*alpha1) / c12;
                    bb1 = hny * aa1;
                        
                    aa2 = 0.25 * (alpha1 - aa1);
                    bb2 = 0.25 * (alpha1 - bb1);
                        
                    v = 0.5 * (1 + std::sqrt(aa1));
                    w = 0.5 * (1 + std::sqrt(bb1));
                    a0 = 0.5 * (v + sqrt(std::pow(v,2.0) + aa2));
                    inva0 = 1/a0;
                        
                    a1 = (1.0 - v) * inva0;
                    a2 = -0.25 * aa2 * std::pow(inva0,2.0);
                        
                    b0 = ((0.5 * (w + sqrt(std::pow(w,2.0) + bb2))) * inva0);
                    b1 = ((1 - w) * inva0);
                    b2 = ((-0.25 * bb2 / b0) * std::pow(inva0,2.0));
                                        
                    break;
                                        
                default: // LPF
                    
                    // Base variables
                    sinrad2 = std::sin(w0/2);
                    q = 1/(2*Q);
                    
                    if (q <= 1) {
                        a1 = (-2 * exp(-q*w0)) * std::cos(std::sqrt(1-(std::pow(q,2.0)))*w0);
                    } else {
                        a1 = (-2 * exp(-q*w0)) * std::cosh(std::sqrt((std::pow(q,2.0))-1)*w0);
                    }
                    
                    a2 = exp(-2 * q * w0);
                    
                    phi0 = 1 - std::pow(sinrad2,2.0);
                    phi1 = std::pow(sinrad2,2.0);
                    phi2 = 4 * phi0 * phi1;
                                
                    A0 = 1 + a1 + a2;
                    A1 = 1 - a1 + a2;
                    A2 = -4 * a2;
                    
                    A0 *= A0; A1 *= A1;
                    
                    // LPF
                    R1 = (A0*phi0 + A1*phi1 + A2*phi2) * std::pow(Q,2.0);
                    B0 = A0;
                    B1 = (R1 - B0*phi0) / phi1;
                    
                    b0 = 0.5 * (std::sqrt(B0) + std::sqrt(B1));
                    b1 = std::sqrt(B0) - b0;
                    b2 = 0.0;

                    break;
                    
            }
            
            break;
    
    }
            
};
