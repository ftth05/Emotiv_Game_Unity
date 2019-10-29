using UnityEngine;

public class ShellExplosion : MonoBehaviour
{
    public LayerMask m_TankMask;
    public ParticleSystem m_ExplosionParticles;       
    public AudioSource m_ExplosionAudio;              
    public float m_MaxDamage = 100f; //perfect hit                 
    public float m_ExplosionForce = 1000f;        
    public float m_MaxLifeTime = 2f; // lifetime         
    public float m_ExplosionRadius = 5f;  // how far from the shell is gonna affect when it explodes.            


    private void Start()
    {
        Destroy(gameObject, m_MaxLifeTime); //destrroy the game object after lifetime
    }


    private void OnTriggerEnter(Collider other)
    {
        // Find all the tanks in an area around the shell and damage them.

        Collider[] colliders = Physics.OverlapSphere(transform.position, m_ExplosionRadius, m_TankMask);

        for(int i = 0; i<colliders.Length; i++)
        {
            Rigidbody targetRigitBody = colliders[i].GetComponent<Rigidbody>();

            if (!targetRigitBody)
                continue;

            targetRigitBody.AddExplosionForce(m_ExplosionForce, transform.position, m_ExplosionRadius);

            TankHealth targetHealth = targetRigitBody.GetComponent<TankHealth>();

            if (!targetHealth) // if the object dont have rigidbody, continue/ skip that one.
                continue;

            float damage = CalculateDamage(targetRigitBody.position);
            targetHealth.TakeDamage(damage);

        }
        // if we remove particals, audio will be alsi removed because of child. it should play even after destroyed the shell. So we do as follow.
        m_ExplosionParticles.transform.parent = null; // unparent the shell/ it will loose in the list of game obj in hierarchy

        m_ExplosionParticles.Play();
        m_ExplosionAudio.Play();

        Destroy(m_ExplosionParticles.gameObject, m_ExplosionParticles.duration);
        Destroy(gameObject);
         
    }


    private float CalculateDamage(Vector3 targetPosition)
    {
        // Calculate the amount of damage a target should take based on it's position.
        //return 0f;
        Vector3 explosionToTarget = targetPosition - transform.position;

        float explosionDistance = explosionToTarget.magnitude; // Pop pop
        float relativeDistance = (m_ExplosionRadius - explosionDistance) / m_ExplosionRadius;
        float damage = relativeDistance * m_MaxDamage;

        damage = Mathf.Max(0f, damage);

        return damage;
    }
}